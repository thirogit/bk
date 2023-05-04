#include "stdafx.h"
#include "SeasonSession.h"
#include <boost/foreach.hpp>
#include <string/TextUtils.h>
#include "seasonimpl/ScopedUpdateState.h"
#include <errortoken/ErrorToken.h>
#include <arrays/DeepDelete.h>
#include <logic/changedetect/DocChangeDetector.h>
#include <logic/changedetect/Diffrence.h>
#include <boost/range/adaptor/transformed.hpp>
#include <data/motives/Motives.h>
#include <logic/changedetect/InvoiceChangeDetector.h>
#include <types/DateRange.h>
#include <utils/Memorandum.h>
#include "index/ServerObjectPeeler.h"
#include "FarmSession.h"
#include "ClientSeasonSession.h"
#include "seasonimpl/SeasonSessionFacadeImpl.h"
#include "seasonimpl/ClientSeasonSessionInterfaceImpl.h"
#include "seasonimpl/operations/InsertPurchaseOperation.h"
#include "db/SQLException.h"
#include <string/ToString.h>

SeasonSession::SeasonSession() : m_pSeason(NULL),m_pFarmSession(NULL),
								 m_pSpectator(NULL),
								 m_State(Session_Idle),m_service(1),m_work(m_service),
								 m_pClientInterface(NULL), m_tokenGenerator(NULL),
								 m_pChangePublisher(NULL)
{
}


SeasonSessionState SeasonSession::GetSessionState()
{
	boost::shared_lock<boost::shared_mutex> lck(m_StateMutex);
	return m_State.value();
}
	
void SeasonSession::EnterFetchState()
{
	//m_logger->i(L"entering fetch state");
	boost::unique_lock<boost::shared_mutex> lck(m_StateMutex);
	do
	{
		if(m_State == Fetch_In_Progress)
		{
			//m_logger->i(L"entering fetch state");
			m_State.increase();
			return;
		}
		else
		{
			if(m_State == Session_Idle)
			{			
				m_State = StateHolder(Fetch_In_Progress);
				m_State.increase();
				m_StateCV.notify_all();
				return;
			}
		}
		m_StateCV.wait(lck);
	}
	while(true);	
}

void SeasonSession::ExitFetchState()
{	
	//m_logger->i(L"exiting fetch state");
	boost::unique_lock<boost::shared_mutex> lck(m_StateMutex);
	if(m_State == Fetch_In_Progress)
	{
		m_State.decrease();
		if(m_State.count() == 0)
		{
			m_State = StateHolder(Session_Idle);			
			m_StateCV.notify_all();
		}
		return;
	}	
	BOOST_THROW_EXCEPTION(SessionException(L"expecting fetch state"));
}

void SeasonSession::EnterUpdateState()
{
	//m_logger->i(L"entering update state");
	boost::unique_lock<boost::shared_mutex> lck(m_StateMutex);
	do
	{
		if(m_State == Update_In_Progress)
		{
			m_State.increase();			
			return;	
		}
		else
		{
			if(m_State == Session_Idle)
			{
				m_State = StateHolder(Update_In_Progress);
				m_State.increase();
				m_StateCV.notify_all();
				return;
			
			}
		}	
		m_StateCV.wait(lck);
	}
	while(true);
}

void SeasonSession::ExitUpdateState()
{
	//m_logger->i(L"exiting state");
	boost::unique_lock<boost::shared_mutex> lck(m_StateMutex);
	if(m_State == Update_In_Progress)
	{
		m_State.decrease();
		if(m_State.count() == 0)
		{
			m_State = StateHolder(Session_Idle);			
			m_StateCV.notify_all();
		}		
		return;
	}	
	BOOST_THROW_EXCEPTION(SessionException(L"expecting update state"));
}

ISeason* SeasonSession::GetSeason() const
{
	return m_pSeason;
}

void SeasonSession::ThrowHentNotFound(uint32_t hentId)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"hent with id=%d not found in season",hentId)));
}

InDocServerObject* SeasonSession::FindInDoc(uint32_t docId)
{
	return m_InDocs.find(docId);
}

InDocServerObject* SeasonSession::AcquireInDoc(uint32_t docId)
{
	InDocServerObject* pDocSO = FindInDoc(docId);
	
	if(!pDocSO)
		ThrowDocNotFound(DocKey(DocType_In,docId));
	return pDocSO;
}

HentServerObject* SeasonSession::FindHent(uint32_t hentId)
{
	boost::multi_index::index<HentsSOIndex, hentsindex_tag_hentId>::type& hentsById = boost::multi_index::get<hentsindex_tag_hentId>(m_Hents);
	auto hentIt = hentsById.find(hentId);
	if (hentIt != hentsById.end())
		return *hentIt;

	return NULL;
}

HentServerObject* SeasonSession::FindHent(const HentNo& hentNo)
{
	boost::multi_index::index<HentsSOIndex, hentsindex_tag_hentNo>::type& hentsByHentNo = boost::multi_index::get<hentsindex_tag_hentNo>(m_Hents);
	auto hentIt = hentsByHentNo.find(hentNo);
	if (hentIt != hentsByHentNo.end())
		return *hentIt;

	return NULL;
}

HentServerObject* SeasonSession::AcquireHent(uint32_t hentId)
{
	HentServerObject* pHentSO = FindHent(hentId);
	if(!pHentSO)
		ThrowHentNotFound(hentId);	

	return pHentSO;
}


void SeasonSession::LockHent(ClientSeasonSession* pClient,uint32_t hentId)
{
	ScopedUpdateState(this);
	m_logger->i(TextUtils::Format(L"locking hent with id=%d", hentId));

	HentServerObject* pHentSO = AcquireHent(hentId);
	if(Lock(pClient,pHentSO))
	{		
		ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
		transaction->OnHentLock(hentId);
		transaction->Commit();
		return;
	}
	ThrowHentAlreadyLockedBy(pHentSO->Object(),pHentSO->GetLockHolder());
}

void SeasonSession::ThrowHentAlreadyLockedBy(IHent* pHent,ClientSeasonSession* pSession)
{
	ClientSession* pClient = pSession->GetClientSession();
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"hent_already_locked")
									 (L"user_id",pClient->GetUserId())
									 (L"hent_id",pHent->GetId())
									 (L"hent_no",pHent->GetHentNo().ToString()).str()));
}

void SeasonSession::UnlockHent(ClientSeasonSession* pClient,uint32_t hentId)
{
	ScopedUpdateState(this);

	HentServerObject* pHentSO = FindHent(hentId);
	if(pHentSO)
	{

		if(!pHentSO->OwnsLock(pClient))
			ThrowLockHighjack();

		Unlock(pClient,pHentSO);	
			
		ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
		transaction->OnHentUnlock(hentId);
		transaction->Commit();			
		return;	
		
	}
	ThrowHentNotFound(hentId);
}


CowServerObject* SeasonSession::FindCow(uint32_t cowId)
{
	return m_Cows.find(cowId);
}

void SeasonSession::LockCow(ClientSeasonSession* pClient,uint32_t cowId)
{
	ScopedUpdateState(this);

	CowServerObject* pCowSO = FindCow(cowId);
	if(pCowSO)
	{
		if(Lock(pClient,pCowSO))
		{
			ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
			transaction->OnCowLock(cowId);
			transaction->Commit();						
			return;
		}
		ThrowCowAlreadyLockedBy(pCowSO->Object(),pCowSO->GetLockHolder());
	}
	ThrowCowNotFound(cowId);	
}

void SeasonSession::ThrowCowAlreadyLockedBy(ICow* pCow,ClientSeasonSession* pSession)
{
	ClientSession* pClient = pSession->GetClientSession();
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"cow_already_locked")
									 (L"user_id",pClient->GetUserId())
									 (L"cow_id",pCow->GetId())
									 (L"cow_no",pCow->GetCowNo().ToString()).str()));
}
																										
void SeasonSession::ThrowCowNotFound(uint32_t cowId)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"cow with id=%d not found in season", cowId)));
}

void SeasonSession::ThrowLockHighjack()
{
	BOOST_THROW_EXCEPTION(SessionException(L"lock_hijack"));
}

void SeasonSession::UnlockCow(ClientSeasonSession* pClient,uint32_t cowId)
{
	ScopedUpdateState(this);

	CowServerObject* pCowSO = AcquireCow(cowId);
	if(!pCowSO->OwnsLock(pClient))
		ThrowLockHighjack();

	Unlock(pClient,pCowSO);
	
	ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
	transaction->OnCowUnlock(cowId);
	transaction->Commit();
	
}

void SeasonSession::ValidateMandatoryForHent(IHent* hent)
{
	if (hent->GetName().empty())
		BOOST_THROW_EXCEPTION(SessionException(L"empty_hent_name"));
	if (hent->GetAlias().empty())
		BOOST_THROW_EXCEPTION(SessionException(L"empty_hent_alias"));
	if (hent->GetStreet().empty())
		BOOST_THROW_EXCEPTION(SessionException(L"empty_hent_street"));
	if (hent->GetPOBox().empty())
		BOOST_THROW_EXCEPTION(SessionException(L"empty_hent_pobox"));
	if (hent->GetCity().empty())
		BOOST_THROW_EXCEPTION(SessionException(L"empty_hent_city"));
	if (hent->GetZip().empty())
		BOOST_THROW_EXCEPTION(SessionException(L"empty_hent_zip"));
	HentType henttype = hent->GetHentType();
	if (henttype != HentType_Company && henttype != HentType_Individual)
		BOOST_THROW_EXCEPTION(SessionException(L"invalid_hent_type"));
}

uint32_t SeasonSession::InsertHent(ClientSeasonSession* pClient,IHent* hent)
{	
	ScopedUpdateState(this);
	m_logger->i(L"inserting new hent: " + ToString(hent));

	try
	{	
		ValidateMandatoryForHent(hent);

		boost::multi_index::index<HentsSOIndex,hentsindex_tag_hentNo>::type& hentsByHentNo = boost::multi_index::get<hentsindex_tag_hentNo>(m_Hents);	
		auto hentIt = hentsByHentNo.find(hent->GetHentNo());
		if(hentIt != hentsByHentNo.end())
		{
			BOOST_THROW_EXCEPTION(SessionException(L"duplicated_hentno"));
		}
		
		try
		{
			Transaction transaction = m_DAO.OpenTransaction();
			Hent* pNewHent = m_DAO.InsertHent(transaction, m_pSeason->GetId(), hent);
			transaction->Commit();

			m_Hents.insert(new HentServerObject(pNewHent));
			ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
			observerTransaction->OnHentInserted(pNewHent);
			observerTransaction->Commit();

			return pNewHent->GetId();
		}
		catch (SQLException& e)
		{
			BOOST_THROW_EXCEPTION(SessionException(e.message()));
		}
	}
	catch (SessionException& e)
	{
		m_logger->e(L"insert hent failed with: " + e.message());
		boost::rethrow_exception(boost::current_exception());
	}
	return NULL_ID;
}


void SeasonSession::UpdateHent(ClientSeasonSession* pClient,IHent* hent)
{
	m_logger->i(TextUtils::Format(L"updating hent(id=%d): ", hent->GetId()) + ToString(hent));
	ScopedUpdateState(this);

	try
	{
		HentServerObject* pHentSO = FindHent(hent->GetId());
		if(pHentSO)
		{
			Hent* pServerSideHent = pHentSO->Object();

			bool bUpdatingHentNo = (pServerSideHent->GetHentNo() != hent->GetHentNo());	
			boost::multi_index::index<HentsSOIndex,hentsindex_tag_hentNo>::type& hentsByHentNo = boost::multi_index::get<hentsindex_tag_hentNo>(m_Hents);

			if(bUpdatingHentNo)
			{	
				if(hentsByHentNo.find(hent->GetHentNo()) != hentsByHentNo.end())
					BOOST_THROW_EXCEPTION(SessionException(L"updating_makes_duplicated_hentno"));
			}

			ValidateMandatoryForHent(hent);
		
			if(pHentSO->OwnsLock(pClient))
			{
				Transaction transaction = m_DAO.OpenTransaction();
				if(m_DAO.UpdateHent(transaction,hent))
				{
					transaction->Commit();
				
					if(bUpdatingHentNo)
					{
						hentsByHentNo.modify(hentsByHentNo.find(pServerSideHent->GetHentNo()),
															   [hent](HentServerObject* pSO)
															   {
																   pSO->Object()->CopyFrom(hent);
															   });
					}
					else
					{
						pServerSideHent->CopyFrom(hent);	
					}

					ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
					transaction->OnHentUpdated(pServerSideHent);
					transaction->Commit();						
					
				}
				else
				{
					BOOST_THROW_EXCEPTION(SessionException(L"db update failed"));
				}	
			}
			else
			{
				BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"hent update id=%d without acquiring the lock", hent->GetId())));
			}
		}
		else
		{
			ThrowHentNotFound(hent->GetId());
		}
	}
	catch(...)
	{		
		boost::rethrow_exception( boost::current_exception() ); 		
	}
}

void SeasonSession::DeleteHent(ClientSeasonSession* pClient,uint32_t hentId)
{
	ScopedUpdateState(this);

	try
	{
		HentServerObject* pHentSO = FindHent(hentId);
		if(pHentSO)
		{
			if(pHentSO->OwnsLock(pClient))
			{
				Transaction transaction = m_DAO.OpenTransaction();
				if(m_DAO.DeleteHent(transaction,pHentSO->Object()))
				{
					transaction->Commit();

					Unlock(pClient, pHentSO);

					m_Hents.erase(m_Hents.key_extractor()(pHentSO));
					delete pHentSO;

					ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
					transaction->OnHentDeleted(hentId);
					transaction->Commit();	
					
				}
				else
				{
					BOOST_THROW_EXCEPTION(SessionException(L"delete hent failed"));
				}
			}
			else
			{
				BOOST_THROW_EXCEPTION(SessionException(L"client session does not own the lock"));
			}
		}
		else
		{
			ThrowHentNotFound(hentId);			
		}
	}
	catch(...)
	{
		boost::rethrow_exception( boost::current_exception() ); 		
	}

}

ClientSeasonSessionFacade SeasonSession::Open(ClientSession* pClient)
{
	uint32_t sessionToken = m_tokenGenerator->ObtainNextToken();
	ClientSeasonSession* pNewClient = new ClientSeasonSession(pClient,m_pClientInterface,sessionToken);
	
	boost::unique_lock<boost::shared_mutex> lck(m_ClientMutex);	
	m_SeasonClients.insert(pNewClient);
	return ClientSeasonSessionFacade(pNewClient);
}

void SeasonSession::Close(ClientSeasonSession* session)
{
	ObserverTransactionPtr transaction = m_pSpectator->OpenTransaction();
	//unlock objects
	
	uint32_t id = NULL_ID;
	LockObject type = LockObject_None;
	do
	{
		ObjectLock objLock = m_lockRecord.PopLock(session);
		id = objLock.GetId();
		type = objLock.GetType();
		switch(type)
		{
		case LockObject_Hent:
			Unlock(session,FindHent(id));
			transaction->OnHentUnlock(id);
			break;
		case LockObject_Cow:
			Unlock(session,FindCow(id));
			transaction->OnCowUnlock(id);
			break;
		case LockObject_InDoc:
			Unlock(session,FindInDoc(id));
			transaction->OnInDocUnlock(id);
			break;
		case LockObject_BuyDoc:
			Unlock(session,FindBuyDoc(id));
			transaction->OnBuyDocUnlock(id);
			break;
		case LockObject_MoveDoc:
			Unlock(session,FindMoveDoc(id));
			transaction->OnMoveDocUnlock(id);
			break;
		case LockObject_SellDoc:
			Unlock(session,FindSellDoc(id));
			transaction->OnSellDocUnlock(id);
			break;
		case LockObject_OutDoc:
			Unlock(session,FindOutDoc(id));
			transaction->OnOutDocUnlock(id);
			break;
		case LockObject_BuyInvoice:
			Unlock(session,FindBuyInvoice(id));
			transaction->OnBuyInvoiceUnlocked(id);
			break;
		case LockObject_SellInvoice:
			Unlock(session,FindSellInvoice(id));
			transaction->OnSellInvoiceUnlocked(id);
			break;
		}
	}
	while(type != LockObject_None);

	transaction->Commit();

	boost::unique_lock<boost::shared_mutex> lck(m_ClientMutex);	
	m_SeasonClients.erase(session);
	delete session;	
}

IHerd* SeasonSession::FindHerdByIndex(int herdIndex)
{
	return m_pFarmSession->FindHerdByIndex(herdIndex);
}

IHerd* SeasonSession::FindHerdById(uint32_t herdId)
{
	return m_pFarmSession->FindHerdById(herdId);
}

void SeasonSession::ThrowDocNotFound(const DocKey& docKey)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"doc key=%c%d not found in season", docKey.GetDocType(), docKey.GetDocNo())));
}

void SeasonSession::LockCowObjects(ClientSeasonSession* pClient,const std::vector<CowServerObject*>& cowObjs)
{
	CowServerObject* pCowSO ;
	std::vector<CowServerObject*> cowCOLockUndo;
	for(std::vector<CowServerObject*>::const_iterator cowIt = cowObjs.begin();cowIt != cowObjs.end();cowIt++)
	{
		pCowSO = *cowIt;
		try
		{
			if(!Lock(pClient,pCowSO))
				ThrowCowAlreadyLockedBy(pCowSO->Object(),pClient);

			cowCOLockUndo.push_back(pCowSO);			
		}
		catch(...)
		{
				std::for_each(cowCOLockUndo.begin(),cowCOLockUndo.end(),[this,pClient](CowServerObject* pCowSO)
																		{
																			Unlock(pClient,pCowSO);
																		});	

				boost::rethrow_exception(boost::current_exception());
			
		}
	}
	
}

void SeasonSession::GetCowSOsForDoc(std::vector<CowServerObject*>& destination, const IDoc* pDoc)
{
	ICowEntry* pCowEntry;
	CowServerObject* pCowSO;
	uint32_t cowId;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	std::vector<CowServerObject*> docCowSOs;
	while(cowsEnum.hasNext())
	{
		pCowEntry = *cowsEnum;
		cowId = pCowEntry->GetCowId();
		pCowSO = FindCow(cowId);
		if(!pCowSO)
		{			
			ThrowCowNotFound(cowId);
		}
		docCowSOs.push_back(pCowSO);
		cowsEnum.advance();
	}

	std::copy(docCowSOs.begin(),docCowSOs.end(),std::back_inserter(destination));
}

std::vector<CowServerObject*> SeasonSession::GetCowSOsForDoc(const IDoc* pDoc)
{
	std::vector<CowServerObject*> cowSOs;
	GetCowSOsForDoc(cowSOs,pDoc);
	return std::move(cowSOs);
}

void SeasonSession::LockInDoc(ClientSeasonSession* pClient, uint32_t docId)
{
	ScopedUpdateState(this);

	InDocServerObject* pDocSO = FindInDoc(docId);
	if (pDocSO)
	{
		IInDoc* pInDoc = pDocSO->Object();
		
		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		if (!Lock(pClient, pDocSO, LockType_Shallow))
			ThrowDocAlreadyLocked(pInDoc, pDocSO->GetLockHolder());
		return;

	}
	ThrowDocNotFound(DocKey(DocType_In, docId));
}


void SeasonSession::LockInDocWithCows(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	InDocServerObject* pDocSO = FindInDoc(docId);	
	if(pDocSO)
	{
		IInDoc* pInDoc = pDocSO->Object();
		std::vector<CowServerObject*>cowSOs(GetCowSOsForDoc(pInDoc));

		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		if(!Lock(pClient,pDocSO,LockType_Deep))
			ThrowDocAlreadyLocked(pInDoc,pDocSO->GetLockHolder());

		try
		{
			LockCowObjects(pClient,cowSOs);
			std::for_each(cowSOs.begin(),cowSOs.end(),[pClient,&observerTransaction](CowServerObject* pCowSO)
													{
														observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
													});
			observerTransaction->OnInDocLock(docId);
			observerTransaction->Commit();
			return;
		}
		catch(...)
		{
			Unlock(pClient,pDocSO);
			boost::rethrow_exception(boost::current_exception());		
		}
	}
	ThrowDocNotFound(DocKey(DocType_In,docId));
}

void SeasonSession::CheckForLockHighjack(std::vector<CowServerObject*>& cowSOs,ClientSeasonSession* pClient)
{
	std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient](CowServerObject* cowSO)
	{
			if(!cowSO->OwnsLock(pClient))
				ThrowLockHighjack();
	});
			
}

void SeasonSession::UnlockInDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);
		
	InDocServerObject* pDocSO = FindInDoc(docId);
	if(pDocSO)
	{
		if(!pDocSO->OwnsLock(pClient))
			ThrowLockHighjack();	

		IInDoc* pDoc = pDocSO->Object();	
		LockType lockType = pDocSO->GetLockType();
		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

		if (lockType == LockType_Deep)
		{
			std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));
			CheckForLockHighjack(cowSOs, pClient);

			std::for_each(cowSOs.begin(), cowSOs.end(), [this, pClient, observerTransaction](CowServerObject* pCowSO)
			{
				Unlock(pClient, pCowSO);
				observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
			});
		}
				
		Unlock(pClient,pDocSO);
		observerTransaction->OnInDocUnlock(docId);		
		observerTransaction->Commit();
		return;
	}
	ThrowDocNotFound(DocKey(DocType_In,docId));
}

void SeasonSession::UpdateInDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot)
{
	ScopedUpdateState(this);

	m_logger->i(L"updating indoc " + snapshot.ToString());

	uint32_t docId = snapshot.GetDocId();
	
	InDocServerObject* pDocSO = FindInDoc(docId);
	if(pDocSO)
	{
		if(!pDocSO->OwnsLock(pClient))
			BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"indoc update id=%d without acquiring the lock", docId)));

		InDoc* pOldDoc = pDocSO->Object();
		Transaction transaction = m_DAO.OpenTransaction();

		std::set<uint32_t> oldCowIds;
		std::vector<Cow*> newCows;
		std::map<uint32_t,CowDetails> cowUpdates;

		PtrEnumerator<ICowEntry> cowsEnum = pOldDoc->EnumCows();
		while(cowsEnum.hasNext())
		{
			oldCowIds.insert((*cowsEnum)->GetCowId());
			cowsEnum.advance();
		}
		
		std::auto_ptr<InDoc> ptrUpdateDoc(new InDoc(pOldDoc->GetId()));
		ptrUpdateDoc->SetDetails(snapshot.GetDetails());
		ptrUpdateDoc->SetHerdId(pOldDoc->GetHerdId());

		BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
		{			
			uint32_t cowId = item->GetCowId();
			if(cowId != NULL_ID)
			{
				Cow* pOldCow = AcquireCow(cowId)->Object();
				CowNo newCowNo = item->GetCow().GetCowNo();
				if(pOldCow->GetCowNo() != newCowNo)
				{
					CheckIfCowNoChangeIsAllowed(cowId,newCowNo);
				}
			}
			else
			{
				CheckForInStockDuplication(pOldDoc->GetHerdId(),item->GetCow().GetCowNo());
			}
		}

		try
		{
			BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
			{
				uint32_t cowId = item->GetCowId();
				if(cowId != NULL_ID)
				{
					oldCowIds.erase(cowId);
					cowUpdates[cowId] = item->GetCow();
					m_DAO.UpdateCow(transaction,cowId,item->GetCow());
					CowEntry* pEntry = ptrUpdateDoc->AddCow(cowId);
					pEntry->SetFrom(item->GetEntry());
				}
				else
				{
					Cow* pNewCow = m_DAO.InsertCow(transaction,m_pSeason->GetId(),item->GetCow());
					newCows.push_back(pNewCow);
					CowEntry* pEntry = ptrUpdateDoc->AddCow(pNewCow->GetId());
					pEntry->SetFrom(item->GetEntry());
				}			
			}
				
			m_DAO.UpdateInDoc(transaction,ptrUpdateDoc.get());

			BOOST_FOREACH(uint32_t deletedCowId,oldCowIds)
			{
				m_DAO.DeleteCow(transaction,deletedCowId);
			}
		}
		catch(...)
		{
			DeepDelete(newCows);
			boost::rethrow_exception(boost::current_exception());
		}

		HerdRegistry* pRegistry = m_Registers.at(pOldDoc->GetHerdId());
		
		for(auto newCowIt = newCows.begin();newCowIt != newCows.end();newCowIt++)
		{
			m_Cows.add(new CowServerObject(*newCowIt));			
		}
			
		
		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());


		pOldDoc->SetDetails(ptrUpdateDoc.get());

		std::set<uint32_t> deletedCowIds,insertedCowIds;
		DocChanges changes = detector.Detect();
		BOOST_FOREACH(DocChange* change,changes)
		{			
			switch(change->Event())
			{
				case ChangeEvent_Added:
				{
					ICowEntry* pAddedEntry = change->New();
					uint32_t cowId = pAddedEntry->GetCowId();
					CowServerObject* pNewCowSO = m_Cows.find(cowId);
					Cow* pNewCow = pNewCowSO->Object();
					observerTransaction->OnCowInserted(pNewCow);
					Lock(pClient,pNewCowSO);
					CowEntry* pNewEntry = pOldDoc->AddCow(cowId);
					pNewEntry->CopyFrom(pAddedEntry);									
					CowTrail* pTrail = m_Tracker.StartTrail(pNewCow);
					pTrail->SetEntryFootprint(pOldDoc,pNewEntry);
					pRegistry->AddEnteringEvent(pNewCow,pOldDoc,pNewEntry);
					m_CowNoReference.AddReference(pNewCow->GetCowNo(),cowId);				

					insertedCowIds.insert(cowId);
					break;
				}
				case ChangeEvent_Deleted:
				{
					ICowEntry* pDeletedEntry = change->Old();
					uint32_t cowId = pDeletedEntry->GetCowId();
					CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
					Cow* pDeletedCow = pDeletedCowSO->Object();
					Unlock(pClient,pDeletedCowSO);
					pOldDoc->RemoveEntry(cowId);
					m_Cows.remove(cowId);
					m_Tracker.DeleteTrail(cowId);
					pRegistry->RemoveEnteringEvent(pDeletedCow,pOldDoc);
					m_CowNoReference.RemoveReference(pDeletedCow->GetCowNo(),cowId);
					delete pDeletedCowSO;
					deletedCowIds.insert(cowId);
					break;
				}
				case ChangeEvent_Updated:
				{
					ICowEntry* pUpdatedEntry = change->New();
					uint32_t cowId = pUpdatedEntry->GetCowId();
					pOldDoc->GetEntry(cowId)->CopyFrom(change->New());
					CowDetails cowUpdate = cowUpdates[cowId];
										
					CowServerObject* pObsoleteCowSO = m_Cows.find(cowId);
					Cow* pObsoleteCow = pObsoleteCowSO->Object();

					m_CowNoReference.RemoveReference(pObsoleteCow->GetCowNo(),cowId);
					pObsoleteCow->SetFrom(cowUpdate);
					m_CowNoReference.AddReference(pObsoleteCow->GetCowNo(),cowId);
					observerTransaction->OnCowUpdated(pObsoleteCow);
					break;
				}
			}
		}

		observerTransaction->OnInDocUpdated(pOldDoc);

		BOOST_FOREACH(uint32_t deletedCowId, deletedCowIds)
		{
			observerTransaction->OnCowDeleted(deletedCowId);
		}

		BOOST_FOREACH(uint32_t insertedCowId, insertedCowIds)
		{
			observerTransaction->OnCowLock(insertedCowId);
		}
		
		transaction->Commit();
		observerTransaction->Commit();

		//m_pChangePublisher->PublishInDocUpdate(pOldDoc);
		return;
	}
	ThrowDocNotFound(DocKey(DocType_In,docId));
}

ICow* SeasonSession::FindCowInDoc(IDoc* pDoc,const CowNo& cowNo)
{
	ICowEntry* pCowEntry;
	ICow* pCow;
	PtrEnumerator<ICowEntry> cowEnum = pDoc->EnumCows();
	while(cowEnum.hasNext())
	{
		pCowEntry = *cowEnum;
		pCow = FindCow(pCowEntry->GetCowId())->Object();
		if(pCow->GetCowNo() == cowNo)
			return pCow;
		cowEnum.advance();
	}
	return NULL;
}

void SeasonSession::ValidateItemCount(const DocSnapshot& snapshot)
{
	if(snapshot.GetItemsCount() == 0)
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"no_items_in_shapshot").str()));
}

void SeasonSession::ValidateOwnDocSnapshot(const OwnDocSnapshot& snapshot)
{
	const DocDetails& details = snapshot.GetDetails();
	ValidateDocDetails(details);

	if(Motives::GetMotiveDirection(details.GetMotive()) != MotiveDirection_In)
		ThrowInvalidMotive();

	ValidateItemCount(snapshot);	
}

void SeasonSession::ThrowInvalidMotive()
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"invalid_in_motive").str()));
}

void SeasonSession::ThrowInvalidAgent(uint32_t agentId)
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"invalid_agent")(L"id", agentId).str()));
}

void SeasonSession::ValidateDocDetails(const DocDetails& details)
{
	if(details.GetDocDate().IsNull())
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"empty_docdate").str()));

	const ISeason* pSeason = GetSeason();

	if(!DateRange(pSeason->GetStartDate(),pSeason->GetEndDate()).IsDateWithin(details.GetDocDate()))
	{
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"docdate_outof_season")
											 (L"season_str",pSeason->GetStartDate().GetDayDate())
											 (L"season_end",pSeason->GetEndDate().GetDayDate())
											 (L"docdate",details.GetDocDate().GetDayDate()).str()));
	}

	uint32_t agentId = details.GetAgentId();
	if(agentId != NULL_ID)
	{
		if (FindAgent(agentId) == NULL)
			ThrowInvalidAgent(agentId);
	}
}

void SeasonSession::InsertInDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot)
{
	m_logger->i(L"inserting indoc: " + snapshot.ToString());
	ValidateOwnDocSnapshot(snapshot);
	
	ScopedUpdateState(this);
	
	HerdRegistry* pHerdRegistry = m_Registers.at(snapshot.GetHerdId());
	std::auto_ptr<InDoc> newInDocDelete;
	
	for(size_t i = 0,count = snapshot.GetItemsCount();i < count;i++)
	{
		const DocCowItem& item = snapshot.GetItem(i);
		const CowDetails& cowDetails = item.GetCow();
		CheckForInStockDuplication(snapshot.GetHerdId(),cowDetails.GetCowNo());
	}

	Transaction transaction = m_DAO.OpenTransaction();
	NewPtrFlushArray<Cow> cows;
	InDoc* pNewInDoc = new InDoc(NULL_ID);
	newInDocDelete.reset(pNewInDoc);
	const DocDetails& docDetails = snapshot.GetDetails();
	pNewInDoc->SetDetails(docDetails);	
	pNewInDoc->SetHerdId(snapshot.GetHerdId());

	
	InDoc* pInsInDoc = NULL;
	Cow* pCow;
	CowEntry* pInDocEntry;
	try
	{
		for (size_t i = 0, count = snapshot.GetItemsCount(); i < count; i++)
		{
			const DocCowItem& item = snapshot.GetItem(i);
			const CowDetails& cowDetails = item.GetCow();
			const CowEntryDetails& entryDetails = item.GetEntry();
			pCow = m_DAO.InsertCow(transaction,m_pSeason->GetId(),cowDetails);
			cows.Add(pCow);

			pInDocEntry = pNewInDoc->AddCow(pCow->GetId());
			pInDocEntry->SetFrom(entryDetails);
		}
		pInsInDoc = m_DAO.InsertInDoc(transaction,m_pSeason->GetId(),pNewInDoc);
	}
	catch(...)
	{
		transaction->Rollback();
		DeepDelete(cows);
		if(pInsInDoc)
			delete pInsInDoc;

		boost::rethrow_exception(boost::current_exception());
	}	
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_InDocs.add(new InDocServerObject(pInsInDoc));
	pHerdRegistry->AddInDoc(pInsInDoc);


	ICowEntry* pEntry;

	for(NewPtrFlushArray<Cow>::iterator cowIt = cows.begin();cowIt != cows.end();cowIt++)
	{
		pCow = *cowIt;
		m_Cows.add(new CowServerObject(pCow));	
		observerTransaction->OnCowInserted(pCow);
		pEntry = pInsInDoc->GetCow(pCow->GetId());
		pHerdRegistry->AddEnteringEvent(pCow,pInsInDoc,pEntry);
		CowTrail* pTrail = m_Tracker.StartTrail(pCow);
		pTrail->SetEntryFootprint(pInsInDoc,pEntry);
		m_CowNoReference.AddReference(pCow->GetCowNo(),pCow->GetId());
	}
	transaction->Commit();
	observerTransaction->OnInDocInserted(pInsInDoc);
	observerTransaction->Commit();
	//m_pChangePublisher->PublishInDocInsert(pInsInDoc);
}

void SeasonSession::DeleteInDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	InDocServerObject* pInDocSO = FindInDoc(docId);
	if(!pInDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"indoc(id=%d) delete without acquiring the lock", docId)));
	IInDoc* pInDoc = pInDocSO->Object();

	HerdRegistry* pHerdRegistry = m_Registers.at(pInDoc->GetHerdId());

	ThrowIfDocCowsAreNotStationary(pInDoc);
	ThrowIfThereAreInvoices(pInDoc);

	Transaction transaction = m_DAO.OpenTransaction();
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	

	m_DAO.DeleteInDoc(transaction,docId);
	observerTransaction->OnInDocDeleted(docId);

	PtrEnumerator<ICowEntry> cowsEnum = pInDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		m_DAO.DeleteCow(transaction,(*cowsEnum)->GetCowId());
		cowsEnum.advance();
	}

	CowServerObject* pCowSO;
	uint32_t cowId;
	Cow* pCow;
	ICowEntry* pEntry;
	cowsEnum = pInDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();
		pCowSO = FindCow(cowId);
		pCow = pCowSO->Object();		
		Unlock(pClient,pCowSO);

		m_Tracker.DeleteTrail(cowId);
		m_CowNoReference.RemoveReference(pCow->GetCowNo(),cowId);
		pHerdRegistry->RemoveEnteringEvent(pCow,pInDoc);		
		observerTransaction->OnCowDeleted(cowId);
		m_Cows.remove(cowId);
		delete pCowSO;

		cowsEnum.advance();
	}

	Unlock(pClient,pInDocSO);
	pHerdRegistry->RemoveInDoc(docId);	
	m_InDocs.remove(docId);
	delete pInDocSO;

	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishInDocDelete(docId);
}

void SeasonSession::ValidateCowDetails(const CowDetails& details)
{
	if(details.GetCowNo().IsNull())
		BOOST_THROW_EXCEPTION(SessionException(L"cowno_cant_be_empty"));

	uint32_t classId = details.GetClassId();

	if(classId == NULL_ID)
		BOOST_THROW_EXCEPTION(SessionException(L"class_cant_be_null"));

	if(FindClass(classId) == NULL)
		BOOST_THROW_EXCEPTION(SessionException(L"invalid_cowclass"));


	uint32_t stockId = details.GetStockId();
	if(stockId != NULL_ID)
	{
		if(FindStock(stockId) == NULL)
			BOOST_THROW_EXCEPTION(SessionException(L"invalid_cowstock"));
	}

	if(details.GetWeight() <= Decimal(0L))
		BOOST_THROW_EXCEPTION(SessionException(L"invalid_weight"));

}

void SeasonSession::UpdateCow(ClientSeasonSession* pClient,uint32_t cowId,const CowDetails& details)
{
	ValidateCowDetails(details);

	ScopedUpdateState(this);
		
	CowServerObject* pCowSO = FindCow(cowId);

	if(!pCowSO)
		ThrowCowNotFound(cowId);

	if(!pCowSO->OwnsLock(pClient))
	{
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"cow update id=%d without acquiring the lock", cowId)));
	}

	Cow* pCow = pCowSO->Object();
		
	CowTrail* pTrail = m_Tracker.GetTrail(cowId);

	bool bChangingCowNo = pCow->GetCowNo() != details.GetCowNo();
	if(bChangingCowNo)
	{
		CowNo newCowNo = details.GetCowNo();	
		
		IDoc* pTrailDoc = pTrail->GetEntry()->GetDoc();
		if(FindCowInDoc(pTrailDoc,newCowNo))
			ThrowChangeInroducesDuplicationInDoc(pTrailDoc,newCowNo);

		PtrEnumerator<ICowMovement> trailEnum = pTrail->GetTrail();
		while(trailEnum.hasNext())
		{
			pTrailDoc = (*trailEnum)->GetDoc();
			if(FindCowInDoc(pTrailDoc,newCowNo))
				ThrowChangeInroducesDuplicationInDoc(pTrailDoc,newCowNo);
			trailEnum.advance();
		}
		
		const ICowFootprint* pExitFootprint = pTrail->GetExit();
		if(pExitFootprint)
		{
			pTrailDoc = pExitFootprint->GetDoc();
			if(FindCowInDoc(pTrailDoc,newCowNo))
				ThrowChangeInroducesDuplicationInDoc(pTrailDoc,newCowNo);
		}
		
		//check duplication in invoices
		//jezeli wciaz jest na stanie w ostanim stadzie, sprawdzic
	}
		
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	Transaction transaction = m_DAO.OpenTransaction();
	m_DAO.UpdateCow(transaction,cowId,details);
	
	if(bChangingCowNo)
	{
		m_CowNoReference.RemoveReference(pCow->GetCowNo(),cowId);
		m_CowNoReference.AddReference(details.GetCowNo(),cowId);
	}
	pCow->SetFrom(details);
	observerTransaction->OnCowUpdated(pCow);

	transaction->Commit();
	observerTransaction->Commit();
	//m_pChangePublisher->PublishCowUpdate(pCow);
}

void SeasonSession::Do(ClientSeasonSession* pClient,std::function<void (SeasonSessionFacade*)>& action)
{
	typedef std::packaged_task<void ()> task_t;

	SeasonSessionFacadeImpl facade(pClient,this);
	std::function<void ()> f = std::bind(action,&facade);

	std::shared_ptr< task_t > copyable_task = std::make_shared< task_t >(f);
	std::future<void> future = copyable_task->get_future();
	m_service.post(std::bind(&task_t::operator(), copyable_task) );	
	future.wait();

	try
	{
		future.get(); //trigger throwing an exception	
	}
	catch (wexception& e)
	{
		m_logger->e(e.message());
		boost::rethrow_exception(boost::current_exception());
	}
}

FetchHandle SeasonSession::OpenFetchHandle()
{
	EnterFetchState();
	FetchHandleImpl* pNewHandle = new FetchHandleImpl(this);
	m_FetchHandles.insert(pNewHandle);
	return FetchHandle(pNewHandle);
}

void SeasonSession::CloseFetchHandle(FetchHandleImpl* pHandle)
{
	ExitFetchState();
	m_FetchHandles.erase(pHandle);
}

IClass* SeasonSession::FindClass(uint32_t id)
{
	return m_pFarmSession->FindClass(id);
}

IStock* SeasonSession::FindStock(uint32_t id)
{
	return m_pFarmSession->FindStock(id);
}
	
IAgent* SeasonSession::FindAgent(uint32_t agentId)
{
	return m_pFarmSession->FindAgent(agentId);
}

void SeasonSession::ThrowOperationCausesDuplicationInHerd(const CowNo& cowNo,uint32_t herdId)
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"op_causes_dupl_in_herd")
		(L"herdIndex", FindHerdById(herdId)->GetHerdIndex())
		(L"cowNo", cowNo.ToString().c_str()).str()));		
}

void SeasonSession::ThrowChangeInroducesDuplicationInDoc(IDoc* pDoc,const CowNo& cowNo)
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"change_intorduces_dupl_in_doc")
		(L"cowNo", cowNo.ToString().c_str()).str()));
	
}

uint16_t SeasonSession::GetObserverPort() const
{
	return m_pSpectator->GetPort();
}

void SeasonSession::LockBuyDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	BuyDocServerObject* pDocSO = FindBuyDoc(docId);
	IBuyDoc* pDoc = pDocSO->Object();
	std::vector<CowServerObject*>cowSOs(GetCowSOsForDoc(pDoc));

	if(pDocSO)
	{
		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		if(!Lock(pClient,pDocSO))
			ThrowDocAlreadyLocked(pDoc,pDocSO->GetLockHolder());

		try
		{
			LockCowObjects(pClient,cowSOs);
			std::for_each(cowSOs.begin(),cowSOs.end(),[pClient,&observerTransaction](CowServerObject* pCowSO)
													{
														observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
													});
			observerTransaction->OnBuyDocLock(docId);
			observerTransaction->Commit();
			return;
		}
		catch(...)
		{
			Unlock(pClient,pDocSO);
			boost::rethrow_exception(boost::current_exception());		
		}
	}
	ThrowDocNotFound(DocKey(DocType_Buy,docId));
}

void SeasonSession::UnlockBuyDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);
		
	BuyDocServerObject* pDocSO = AcquireBuyDoc(docId);
	
	if(!pDocSO->OwnsLock(pClient))
		ThrowLockHighjack();	

	IBuyDoc* pDoc = pDocSO->Object();
	std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));		
	CheckForLockHighjack(cowSOs,pClient);

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
												{
													Unlock(pClient,pCowSO);
													observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
												});
				
	Unlock(pClient,pDocSO);
	observerTransaction->OnBuyDocUnlock(docId);		
	observerTransaction->Commit();
	
}

void SeasonSession::UpdateBuyDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot)
{
	ScopedUpdateState(this);

	uint32_t docId = snapshot.GetDocId();
	
	BuyDocServerObject* pDocSO = AcquireBuyDoc(docId);
	
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"buydoc update id=%d without acquiring the lock", docId)));

	BuyDoc* pOldDoc = pDocSO->Object();
	Transaction transaction = m_DAO.OpenTransaction();

	std::set<uint32_t> oldCowIds;
	std::vector<Cow*> newCows;
	std::map<uint32_t,CowDetails> cowUpdates;

	PtrEnumerator<ICowEntry> cowsEnum = pOldDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		oldCowIds.insert((*cowsEnum)->GetCowId());
		cowsEnum.advance();
	}
		
	std::auto_ptr<BuyDoc> ptrUpdateDoc(new BuyDoc(pOldDoc->GetId()));
	ptrUpdateDoc->SetDetails(snapshot.GetDetails());
	ptrUpdateDoc->SetHerdId(pOldDoc->GetHerdId());
	ptrUpdateDoc->SetHentId(snapshot.GetHentId());

	BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
	{			
		uint32_t cowId = item->GetCowId();
		if(cowId != NULL_ID)
		{
			Cow* pOldCow = AcquireCow(cowId)->Object();
			CowNo newCowNo = item->GetCow().GetCowNo();
			if(pOldCow->GetCowNo() != newCowNo)
			{
				CheckIfCowNoChangeIsAllowed(cowId,newCowNo);
			}
		}
		else
		{
			CheckForInStockDuplication(pOldDoc->GetHerdId(),item->GetCow().GetCowNo());
		}
	}

	try
	{
		BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
		{
			uint32_t cowId = item->GetCowId();
			if(cowId != NULL_ID)
			{
				oldCowIds.erase(cowId);
				cowUpdates[cowId] = item->GetCow();
				m_DAO.UpdateCow(transaction,cowId,item->GetCow());
				CowEntry* pEntry = ptrUpdateDoc->AddCow(cowId);
				pEntry->SetFrom(item->GetEntry());
			}
			else
			{
				Cow* pNewCow = m_DAO.InsertCow(transaction,m_pSeason->GetId(),item->GetCow());
				newCows.push_back(pNewCow);
				CowEntry* pEntry = ptrUpdateDoc->AddCow(pNewCow->GetId());
				pEntry->SetFrom(item->GetEntry());
			}			
		}
				
		m_DAO.UpdateBuyDoc(transaction,ptrUpdateDoc.get());

		BOOST_FOREACH(uint32_t deletedCowId,oldCowIds)
		{
			m_DAO.DeleteCow(transaction,deletedCowId);
		}
	}
	catch(...)
	{
		DeepDelete(newCows);
		boost::rethrow_exception(boost::current_exception());
	}

	HerdRegistry* pRegistry = m_Registers.at(pOldDoc->GetHerdId());
		
	for(auto newCowIt = newCows.begin();newCowIt != newCows.end();newCowIt++)
	{
		m_Cows.add(new CowServerObject(*newCowIt));			
	}
	
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());

	pOldDoc->SetDetails(ptrUpdateDoc.get());
	pOldDoc->SetHentId(ptrUpdateDoc->GetHentId());

	std::set<uint32_t> deletedCowIds,insertedCowIds;
	DocChanges changes = detector.Detect();
	BOOST_FOREACH(DocChange* change,changes)
	{			
		switch(change->Event())
		{
			case ChangeEvent_Added:
			{
				ICowEntry* pAddedEntry = change->New();
				uint32_t cowId = pAddedEntry->GetCowId();				
				CowServerObject* pNewCowSO = m_Cows.find(cowId);
				Cow* pNewCow = pNewCowSO->Object();
				observerTransaction->OnCowInserted(pNewCow);
				Lock(pClient,pNewCowSO);
				CowEntry* pNewEntry = pOldDoc->AddCow(cowId);
				pNewEntry->CopyFrom(pAddedEntry);									
				CowTrail* pTrail = m_Tracker.StartTrail(pNewCow);
				pTrail->SetEntryFootprint(pOldDoc,pNewEntry);
				pRegistry->AddEnteringEvent(pNewCow,pOldDoc,pNewEntry);
				m_CowNoReference.AddReference(pNewCow->GetCowNo(),cowId);				

				insertedCowIds.insert(cowId);
				break;
			}
			case ChangeEvent_Deleted:
			{
				ICowEntry* pDeletedEntry = change->Old();
				uint32_t cowId = pDeletedEntry->GetCowId();
				CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
				Cow* pDeletedCow = pDeletedCowSO->Object();
				Unlock(pClient,pDeletedCowSO);
				pOldDoc->RemoveEntry(cowId);
				m_Cows.remove(cowId);
				m_Tracker.DeleteTrail(cowId);
				pRegistry->RemoveEnteringEvent(pDeletedCow,pOldDoc);
				m_CowNoReference.RemoveReference(pDeletedCow->GetCowNo(),cowId);
				delete pDeletedCowSO;
				deletedCowIds.insert(cowId);
				break;
			}
			case ChangeEvent_Updated:
			{
				ICowEntry* pUpdatedEntry = change->New();
				uint32_t cowId = pUpdatedEntry->GetCowId();
				pOldDoc->GetEntry(cowId)->CopyFrom(change->New());
				CowDetails cowUpdate = cowUpdates[cowId];

				CowServerObject* pObsoleteCowSO = m_Cows.find(cowId);
				Cow* pObsoleteCow = pObsoleteCowSO->Object();

				m_CowNoReference.RemoveReference(pObsoleteCow->GetCowNo(),cowId);
				pObsoleteCow->SetFrom(cowUpdate);
				m_CowNoReference.AddReference(pObsoleteCow->GetCowNo(),cowId);
				observerTransaction->OnCowUpdated(pObsoleteCow);
				break;
			}
		}
	}

	observerTransaction->OnBuyDocUpdated(pOldDoc);

	BOOST_FOREACH(uint32_t deletedCowId, deletedCowIds)
	{
		observerTransaction->OnCowDeleted(deletedCowId);
	}

	BOOST_FOREACH(uint32_t insertedCowId, insertedCowIds)
	{
		observerTransaction->OnCowLock(insertedCowId);
	}
		
	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishBuyDocUpdate(pOldDoc);
	
}

void SeasonSession::ValidateHentsDocSnapshot(const HentsDocSnapshot& snapshot)
{
	const DocDetails& docDetails = snapshot.GetDetails();
	ValidateDocDetails(docDetails);

	uint32_t hentId = snapshot.GetHentId();
	if(!FindHent(hentId))
		ThrowHentNotFound(hentId);

	ValidateItemCount(snapshot);

}

void SeasonSession::ValidateBuyDocSnapshot(const HentsDocSnapshot& snapshot)
{
	if(Motives::GetMotiveDirection(snapshot.GetDetails().GetMotive()) != MotiveDirection_In)
		ThrowInvalidMotive();

	ValidateHentsDocSnapshot(snapshot);

}

CowIds SeasonSession::GetCowOccurences(const CowNo& cowNo)
{
	return m_CowNoReference.GetCowsWithCowNo(cowNo);
}

void SeasonSession::CheckForInStockDuplication(uint32_t herdId,const CowNo& cowNo)
{
	HerdRegistry* pHerdRegistry = m_Registers.at(herdId);
	CowIds cowIds = GetCowOccurences(cowNo);
		
		auto duplicateIt = std::find_if(cowIds.begin(),cowIds.end(),
							[pHerdRegistry](uint32_t cowId) -> bool 
							{
								PtrEnumerator<ICowPresence> presecnceEnum = pHerdRegistry->GetCowPresences(cowId);
								while(presecnceEnum.hasNext())
								{
									if((*presecnceEnum)->IsInStock())
										return true;
									presecnceEnum.advance();
								}
								return false;
							});
		if(duplicateIt != cowIds.end())
			ThrowOperationCausesDuplicationInHerd(cowNo,herdId);
}

void SeasonSession::InsertBuyDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot)
{
	ValidateBuyDocSnapshot(snapshot);
	
	ScopedUpdateState(this);
	
	HerdRegistry* pHerdRegistry = m_Registers.at(snapshot.GetHerdId());
	std::auto_ptr<BuyDoc> newBuyDocDelete;
	
	for (size_t i = 0, count = snapshot.GetItemsCount(); i < count; i++)
	{
		const DocCowItem& item = snapshot.GetItem(i);
		const CowDetails& cowDetails = item.GetCow();
		CheckForInStockDuplication(snapshot.GetHerdId(),cowDetails.GetCowNo());			
	}

	Transaction transaction = m_DAO.OpenTransaction();
	NewPtrFlushArray<Cow> cows;
	BuyDoc* pNewBuyDoc = new BuyDoc(NULL_ID);
	newBuyDocDelete.reset(pNewBuyDoc);
	const DocDetails& docDetails = snapshot.GetDetails();
	pNewBuyDoc->SetDetails(docDetails);	
	pNewBuyDoc->SetHerdId(snapshot.GetHerdId());
	pNewBuyDoc->SetHentId(snapshot.GetHentId());
	
	BuyDoc* pInsBuyDoc = NULL;
	Cow* pCow;
	CowEntry* pBuyDocEntry;
	try
	{
		for(size_t i = 0,count = snapshot.GetItemsCount();i < count;i++)
		{
			const DocCowItem& item = snapshot.GetItem(i);
			const CowDetails& cowDetails = item.GetCow();
			const CowEntryDetails& entryDetails = item.GetEntry();
			pCow = m_DAO.InsertCow(transaction,m_pSeason->GetId(),cowDetails);
			cows.Add(pCow);

			pBuyDocEntry = pNewBuyDoc->AddCow(pCow->GetId());
			pBuyDocEntry->SetFrom(entryDetails);
		}
		pInsBuyDoc = m_DAO.InsertBuyDoc(transaction,m_pSeason->GetId(),pNewBuyDoc);
	}
	catch(...)
	{
		transaction->Rollback();
		DeepDelete(cows);
		if(pInsBuyDoc)
			delete pInsBuyDoc;

		boost::rethrow_exception(boost::current_exception());
	}	
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_BuyDocs.add(new BuyDocServerObject(pInsBuyDoc));

	pHerdRegistry->AddBuyDoc(pInsBuyDoc);

	ICowEntry* pEntry = NULL;
	for(NewPtrFlushArray<Cow>::iterator cowIt = cows.begin();cowIt != cows.end();cowIt++)
	{
		pCow = *cowIt;
		m_Cows.add(new CowServerObject(pCow));	
		observerTransaction->OnCowInserted(pCow);
		pEntry = pInsBuyDoc->GetCow(pCow->GetId());
		
		pHerdRegistry->AddEnteringEvent(pCow,pInsBuyDoc,pEntry);

		CowTrail* pTrail = m_Tracker.StartTrail(pCow);
		pTrail->SetEntryFootprint(pInsBuyDoc,pEntry);
		
		m_CowNoReference.AddReference(pCow->GetCowNo(),pCow->GetId());
	}
	transaction->Commit();
	observerTransaction->OnBuyDocInserted(pInsBuyDoc);
	observerTransaction->Commit();

	//m_pChangePublisher->PublishBuyDocInsert(pInsBuyDoc);
}

void SeasonSession::ThrowIfDocCowsAreNotStationary(IDoc* pDoc)
{
	Cow* pCow;
	ICowEntry* pEntry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		CowTrail* pTrail = m_Tracker.GetTrail(pEntry->GetCowId());
		if(!pTrail->IsStationary())
		{
			pCow = FindCow(pEntry->GetCowId())->Object();
			BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"cow_not_in_stock")(L"cowno", pCow->GetCowNo().ToString()).str()));
		}
		cowsEnum.advance();
	}
}

void SeasonSession::DeleteBuyDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	BuyDocServerObject* pDocSO = FindBuyDoc(docId);
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"buydoc(id=%d) delete without acquiring the lock", docId)));
	IBuyDoc* pDoc = pDocSO->Object();

	HerdRegistry* pHerdRegistry = m_Registers.at(pDoc->GetHerdId());

	ThrowIfThereAreInvoices(pDoc);

	ThrowIfDocCowsAreNotStationary(pDoc);

	Transaction transaction = m_DAO.OpenTransaction();
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	m_DAO.DeleteBuyDoc(transaction,docId);
	observerTransaction->OnBuyDocDeleted(docId);

	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		m_DAO.DeleteCow(transaction,(*cowsEnum)->GetCowId());
		cowsEnum.advance();
	}

	CowServerObject* pCowSO;
	uint32_t cowId;	
	Cow* pCow;
	ICowEntry* pEntry;

	cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();
		pCowSO = FindCow(cowId);
		pCow = pCowSO->Object();
		Unlock(pClient,pCowSO);
		m_Tracker.DeleteTrail(cowId);
		m_CowNoReference.RemoveReference(pCow->GetCowNo(),cowId);
		pHerdRegistry->RemoveEnteringEvent(pCow,pDoc);		
		observerTransaction->OnCowDeleted(cowId);
		m_Cows.remove(cowId);
		delete pCowSO;

		cowsEnum.advance();
	}

	Unlock(pClient,pDocSO);
	pHerdRegistry->RemoveBuyDoc(docId);
	m_BuyDocs.remove(docId);
	delete pDocSO;

	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishBuyDocDelete(docId);
}

BuyDocServerObject* SeasonSession::AcquireBuyDoc(uint32_t docId)
{
	BuyDocServerObject* pDocSO = FindBuyDoc(docId);
	
	if(!pDocSO)
		ThrowDocNotFound(DocKey(DocType_Buy,docId));
	return pDocSO;
}

BuyDocServerObject* SeasonSession::FindBuyDoc(uint32_t docId)
{
	return m_BuyDocs.find(docId);
}

void SeasonSession::LockMoveDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	MoveDocServerObject* pDocSO = FindMoveDoc(docId);
	
	if(pDocSO)
	{
		IMoveDoc* pDoc = pDocSO->Object();
		std::vector<CowServerObject*>cowSOs(GetCowSOsForDoc(pDoc));

		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		if(!Lock(pClient,pDocSO))
			ThrowDocAlreadyLocked(pDoc,pDocSO->GetLockHolder());

		try
		{
			LockCowObjects(pClient,cowSOs);
			std::for_each(cowSOs.begin(),cowSOs.end(),[pClient,&observerTransaction](CowServerObject* pCowSO)
													{
														observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
													});
			observerTransaction->OnMoveDocLock(docId);
			observerTransaction->Commit();
			return;
		}
		catch(...)
		{
			Unlock(pClient,pDocSO);
			boost::rethrow_exception(boost::current_exception());		
		}
	}
	ThrowDocNotFound(DocKey(DocType_Move,docId));
}

void SeasonSession::UnlockMoveDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);
		
	MoveDocServerObject* pDocSO = FindMoveDoc(docId);
	if(pDocSO)
	{
		if(!pDocSO->OwnsLock(pClient))
			ThrowLockHighjack();	

		IMoveDoc* pDoc = pDocSO->Object();
		std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));		
		CheckForLockHighjack(cowSOs,pClient);

		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
													{
														Unlock(pClient,pCowSO);
														observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
													});

				
		Unlock(pClient,pDocSO);
		observerTransaction->OnMoveDocUnlock(docId);		
		observerTransaction->Commit();
		return;
	}
	ThrowDocNotFound(DocKey(DocType_Move,docId));
}

MoveDocServerObject* SeasonSession::FindMoveDoc(uint32_t docId)
{
	return m_MoveDocs.find(docId);
}

void SeasonSession::UpdateMoveDoc(ClientSeasonSession* pClient,const MoveDocSnapshot& snapshot)
{
	ScopedUpdateState(this);

	ValidateMoveDocSnapshot(snapshot);

	uint32_t docId = snapshot.GetDocId();
	
	MoveDocServerObject* pDocSO = FindMoveDoc(docId);
	if(pDocSO)
	{
		if (!pDocSO->OwnsLock(pClient))
			BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"movedoc(id=%d) update without acquiring the lock", docId)));

		MoveDoc* pOldDoc = pDocSO->Object();
		Transaction transaction = m_DAO.OpenTransaction();

		std::set<uint32_t> oldCowIds;
		
		PtrEnumerator<ICowEntry> cowsEnum = pOldDoc->EnumCows();
		while(cowsEnum.hasNext())
		{
			oldCowIds.insert((*cowsEnum)->GetCowId());
			cowsEnum.advance();
		}

		std::auto_ptr<MoveDoc> ptrUpdateDoc(new MoveDoc(pOldDoc->GetId()));
		ptrUpdateDoc->SetDetails(snapshot.GetDetails());
		ptrUpdateDoc->SetSrcHerdId(pOldDoc->GetSrcHerdId());
		ptrUpdateDoc->SetDstHerdId(pOldDoc->GetDstHerdId());
		
		try
		{
			BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
			{
				uint32_t cowId = item->GetCowId();
				CowEntry* pEntry = ptrUpdateDoc->AddCow(cowId);
				pEntry->SetFrom(item->GetEntry());				
			}				

			DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());		
			DocChanges changes = detector.Detect();
			BOOST_FOREACH(DocChange* change,changes)
			{
				if(change->Event() == ChangeEvent_Deleted)
				{
					ICowEntry* pDeletedEntry = change->Old();
					uint32_t cowId = pDeletedEntry->GetCowId();
					CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
					Cow* pDeletedCow = pDeletedCowSO->Object();
					CheckForInStockDuplication(pOldDoc->GetSrcHerdId(),pDeletedCow->GetCowNo());					
				}
			}

			m_DAO.UpdateMoveDoc(transaction,ptrUpdateDoc.get());			
		}
		catch(...)
		{
			boost::rethrow_exception(boost::current_exception());
		}

		HerdRegistry* pSrcRegistry = m_Registers.at(pOldDoc->GetSrcHerdId());		
		HerdRegistry* pDstRegistry = m_Registers.at(pOldDoc->GetDstHerdId());		
		
		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		pOldDoc->SetDetails(ptrUpdateDoc.get());
		
		DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());		
		DocChanges changes = detector.Detect();
		std::vector<CowServerObject*> deletedCows;
		BOOST_FOREACH(DocChange* change,changes)
		{			
			switch(change->Event())
			{
				case ChangeEvent_Added:
				{
					ICowEntry* pAddedEntry = change->New();
					uint32_t cowId = pAddedEntry->GetCowId();

					CowServerObject* pAddedCowSO = m_Cows.find(cowId);
					Cow* pAddedCow = pAddedCowSO->Object();					
					Lock(pClient,pAddedCowSO);

					CowEntry* pNewEntry = pOldDoc->AddCow(cowId);
					pNewEntry->CopyFrom(pAddedEntry);									

					CowTrail* pTrail = m_Tracker.GetTrail(cowId);
					pTrail->AddFootprint(pOldDoc,pNewEntry);

					pSrcRegistry->AddLeavingEvent(pAddedCow,pOldDoc,pNewEntry);
					pDstRegistry->AddEnteringEvent(pAddedCow,pOldDoc,pNewEntry);		
					break;
				}
				case ChangeEvent_Deleted:
				{
					ICowEntry* pDeletedEntry = change->Old();
					uint32_t cowId = pDeletedEntry->GetCowId();
					CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
					Cow* pDeletedCow = pDeletedCowSO->Object();
										
					CowTrail* pTrail = m_Tracker.GetTrail(cowId);
					pTrail->EraseLastMovement();

					pDstRegistry->RemoveEnteringEvent(pDeletedCow,pOldDoc);
					pSrcRegistry->RemoveLeavingEvent(pDeletedCow,pOldDoc);

					pOldDoc->RemoveEntry(cowId);
					deletedCows.push_back(pDeletedCowSO);
					
					break;
				}
				case ChangeEvent_Updated:
				{
					ICowEntry* pUpdatedEntry = change->New();
					uint32_t cowId = pUpdatedEntry->GetCowId();
					pOldDoc->GetEntry(cowId)->CopyFrom(change->New());			
					break;
				}
			}
		}

		observerTransaction->OnMoveDocUpdated(pOldDoc);

		BOOST_FOREACH(CowServerObject* pDeletedCowSO,deletedCows)
		{
			Unlock(pClient,pDeletedCowSO);
			observerTransaction->OnCowUnlock(pDeletedCowSO->Object()->GetId());
		}
		
		transaction->Commit();
		observerTransaction->Commit();

		//m_pChangePublisher->PublishMoveDocUpdate(pOldDoc);

		return;
	}
	ThrowDocNotFound(DocKey(DocType_Move,docId));
}

void SeasonSession::ValidateMoveDocSnapshot(const MoveDocSnapshot& snapshot)
{
	ValidateDocDetails(snapshot.GetDetails());	
}

void SeasonSession::InsertMoveDoc(ClientSeasonSession* pClient,const MoveDocSnapshot& snapshot)
{
	ValidateMoveDocSnapshot(snapshot);
	
	ScopedUpdateState(this);
	
	HerdRegistry* pSrcHerdRegistry = m_Registers.at(snapshot.GetSrcHerdId());
	HerdRegistry* pDstHerdRegistry = m_Registers.at(snapshot.GetDstHerdId());
	
	std::auto_ptr<MoveDoc> newMoveDocDelete;
	
	for (size_t i = 0, count = snapshot.GetItemsCount(); i < count; i++)
	{
		const DocCowItem& item = snapshot.GetItem(i);
		const CowDetails& cowDetails = item.GetCow();

		CowIds cowIds = m_CowNoReference.GetCowsWithCowNo(cowDetails.GetCowNo());
		
		auto duplicateIt = std::find_if(cowIds.begin(),cowIds.end(),
							[pDstHerdRegistry](uint32_t cowId) -> bool 
							{
								PtrEnumerator<ICowPresence> presecnceEnum = pDstHerdRegistry->GetCowPresences(cowId);
								while(presecnceEnum.hasNext())
								{
									if((*presecnceEnum)->IsInStock())
										return true;

									presecnceEnum.advance();
								}
								return false;
							});
		if(duplicateIt != cowIds.end())
			ThrowOperationCausesDuplicationInHerd(cowDetails.GetCowNo(),snapshot.GetDstHerdId());
			
	}

	Transaction transaction = m_DAO.OpenTransaction();
	NewPtrFlushArray<CowServerObject> cowSOs;
	MoveDoc* pNewMovDoc = new MoveDoc(NULL_ID);
	newMoveDocDelete.reset(pNewMovDoc);
	const DocDetails& docDetails = snapshot.GetDetails();
	pNewMovDoc->SetDetails(docDetails);	
	pNewMovDoc->SetSrcHerdId(snapshot.GetSrcHerdId());
	pNewMovDoc->SetDstHerdId(snapshot.GetDstHerdId());
		
	MoveDoc* pInsMovDoc = NULL;	
	CowEntry* pMovDocEntry;
	try
	{
		uint32_t cowId;
		for (size_t i = 0, count = snapshot.GetItemsCount(); i < count; i++)
		{
			const DocCowItem& item = snapshot.GetItem(i);
			const CowDetails& cowDetails = item.GetCow();
			const CowEntryDetails& entryDetails = item.GetEntry();
			cowId = item.GetCowId();
			CowServerObject* pCowSO = FindCow(cowId);			
			if(!pCowSO->OwnsLock(pClient))
				ThrowCowLockNotObtained(cowId);
			
			m_DAO.UpdateCow(transaction,cowId,cowDetails);
			cowSOs.Add(pCowSO);
			pMovDocEntry = pNewMovDoc->AddCow(cowId);
			pMovDocEntry->SetFrom(entryDetails);
		}
		pInsMovDoc = m_DAO.InsertMoveDoc(transaction,m_pSeason->GetId(),pNewMovDoc);
	}
	catch(...)
	{
		transaction->Rollback();		
		if(pInsMovDoc)
			delete pInsMovDoc;

		boost::rethrow_exception(boost::current_exception());
	}	
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_MoveDocs.add(new MoveDocServerObject(pInsMovDoc));
	observerTransaction->OnMoveDocInserted(pInsMovDoc);

	pSrcHerdRegistry->AddMoveDoc(pInsMovDoc);
	pDstHerdRegistry->AddMoveDoc(pInsMovDoc);
	
	uint32_t cowId;
	CowTrail* pTrail;
	ICowEntry* pEntry;
	ICow* pCow;

	for(NewPtrFlushArray<CowServerObject>::iterator cowIt = cowSOs.begin();cowIt != cowSOs.end();cowIt++)
	{
		CowServerObject* pCowSO = *cowIt;
		pCow = pCowSO->Object();
		cowId = pCow->GetId();
		pTrail = m_Tracker.GetTrail(cowId);
		pEntry = pInsMovDoc->GetCow(cowId);
		pTrail->AddFootprint(pInsMovDoc,pEntry);		

		pSrcHerdRegistry->AddLeavingEvent(pCow,pInsMovDoc,pEntry);
		pDstHerdRegistry->AddEnteringEvent(pCow,pInsMovDoc,pEntry);
		//observerTransaction->OnCowUpdated(pCow);
	}
	
	transaction->Commit();	
	observerTransaction->Commit();		


	//m_pChangePublisher->PublishMoveDocInsert(pInsMovDoc);
}

void SeasonSession::ThrowCowLockNotObtained(uint32_t cowId)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"lock on cow with id = %d not obtained", cowId)));
}

CowServerObject* SeasonSession::AcquireCow(uint32_t cowId)
{
	CowServerObject* pCowSO = FindCow(cowId);
	if(pCowSO)
		return pCowSO;
	ThrowCowNotFound(cowId);
	return NULL;
}

void SeasonSession::CheckIfCowWasMovedFromHerd(uint32_t herdId,const CowPresenceKey& key)
{
	HerdRegistry* pHerdRegistry = m_Registers.at(herdId);
	ICowPresence* pPresence = pHerdRegistry->GetCowPresence(key);
	if(!pPresence)
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"cow(id=%d) was never present in herd(id=%d) via %c%05d",
												  key.GetCowId(),herdId,key.GetDocType(),key.GetDocId())));

	if(!pPresence->IsInStock())
	{
		CowServerObject* pCowSO = AcquireCow(key.GetCowId());
		ICow* pCow = pCowSO->Object();
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"cow_was_moved")(L"cowNo", pCow->GetCowNo().ToString()).str()));
	}
}

MoveDocServerObject* SeasonSession::AcquireMoveDoc(uint32_t docId)
{
	MoveDocServerObject* pMoveDocSO = FindMoveDoc(docId);
	if(!pMoveDocSO)
		ThrowDocNotFound(DocKey(DocType_Move,docId));
	return pMoveDocSO;
}

void SeasonSession::DeleteMoveDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	MoveDocServerObject* pDocSO = AcquireMoveDoc(docId);
	
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"movedoc(id=%d) delete without acquiring the lock", docId)));

	MoveDoc* pDoc = pDocSO->Object();

	uint32_t srcHerdId = pDoc->GetSrcHerdId();
	uint32_t dstHerdId = pDoc->GetDstHerdId();

	HerdRegistry* pSrcHerdRegistry = m_Registers.at(srcHerdId);
	HerdRegistry* pDstHerdRegistry = m_Registers.at(dstHerdId);
	std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));
	Cow* pCow;
	try
	{		
		auto cowIt = cowSOs.begin();
		while(cowIt != cowSOs.end())
		{
			pCow = (*cowIt)->Object();			
			CheckIfCowWasMovedFromHerd(dstHerdId,CowPresenceKey(pCow->GetId(),pDoc->GetDocType(),pDoc->GetId()));
			CheckForInStockDuplication(srcHerdId,pCow->GetCowNo());		
			cowIt++;
		}
	}	
	catch(...)
	{
		boost::rethrow_exception(boost::current_exception());
	}

	Transaction transaction = m_DAO.OpenTransaction();
	m_DAO.DeleteMoveDoc(transaction,docId);
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	uint32_t cowId;
	auto cowIt = cowSOs.begin();
	while(cowIt != cowSOs.end())
	{
		pCow = (*cowIt)->Object();
		cowId = pCow->GetId();
		CowTrail* pTrail = m_Tracker.GetTrail(cowId);
		pTrail->EraseLastMovement();
		pDstHerdRegistry->RemoveEnteringEvent(pCow,pDoc);		
		pSrcHerdRegistry->RemoveLeavingEvent(pCow,pDoc);		
		cowIt++;
	}
	pSrcHerdRegistry->RemoveMoveDoc(docId);
	pDstHerdRegistry->RemoveMoveDoc(docId);
	observerTransaction->OnMoveDocDeleted(docId);
	Unlock(pClient,pDocSO);
	m_MoveDocs.remove(docId);
	delete pDocSO;

	std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
													{
														Unlock(pClient,pCowSO);
														observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
													});
	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishMoveDocDelete(docId);
	
}

SellDocServerObject* SeasonSession::FindSellDoc(uint32_t docId)
{
	return m_SellDocs.find(docId);
}

SellDocServerObject* SeasonSession::AcquireSellDoc(uint32_t docId)
{
	SellDocServerObject* pDocSO = FindSellDoc(docId);
	
	if(!pDocSO)
		ThrowDocNotFound(DocKey(DocType_Sell,docId));
	return pDocSO;
}

void SeasonSession::LockSellDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	SellDocServerObject* pDocSO = AcquireSellDoc(docId);
	
	
	ISellDoc* pDoc = pDocSO->Object();
	std::vector<CowServerObject*>cowSOs(GetCowSOsForDoc(pDoc));

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	if(!Lock(pClient,pDocSO))
		ThrowDocAlreadyLocked(pDoc,pDocSO->GetLockHolder());

	try
	{
		LockCowObjects(pClient,cowSOs);
		std::for_each(cowSOs.begin(),cowSOs.end(),[pClient,&observerTransaction](CowServerObject* pCowSO)
												{
													observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
												});
		observerTransaction->OnSellDocLock(docId);
		observerTransaction->Commit();
		return;
	}
	catch(...)
	{
		Unlock(pClient,pDocSO);
		boost::rethrow_exception(boost::current_exception());		
	}
	
}

void SeasonSession::UnlockSellDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);
		
	SellDocServerObject* pDocSO = FindSellDoc(docId);
	if(pDocSO)
	{
		if(!pDocSO->OwnsLock(pClient))
			ThrowLockHighjack();
		
		ISellDoc* pDoc = pDocSO->Object();
		std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));		
		CheckForLockHighjack(cowSOs,pClient);
		
		ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
		std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
													{
														Unlock(pClient,pCowSO);
														observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
													});

				
		Unlock(pClient,pDocSO);
		observerTransaction->OnSellDocUnlock(docId);		
		observerTransaction->Commit();
		return;
	}
	ThrowDocNotFound(DocKey(DocType_Sell,docId));
}

void SeasonSession::ValidateSellDocSnapshot(const HentsDocSnapshot& snapshot)
{
	if(Motives::GetMotiveDirection(snapshot.GetDetails().GetMotive()) != MotiveDirection_Out)
		ThrowInvalidMotive();

	ValidateHentsDocSnapshot(snapshot);
}


void SeasonSession::UpdateSellDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot)
{
	ScopedUpdateState(this);

	ValidateSellDocSnapshot(snapshot);

	uint32_t docId = snapshot.GetDocId();
	
	SellDocServerObject* pDocSO = AcquireSellDoc(docId);
		
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"selldoc(id=%d) update without acquiring the lock", docId)));

	SellDoc* pOldDoc = pDocSO->Object();
	Transaction transaction = m_DAO.OpenTransaction();

	std::set<uint32_t> oldCowIds;
		
	PtrEnumerator<ICowEntry> cowsEnum = pOldDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		oldCowIds.insert((*cowsEnum)->GetCowId());
		cowsEnum.advance();
	}
		
	std::auto_ptr<SellDoc> ptrUpdateDoc(new SellDoc(pOldDoc->GetId()));
	
	ptrUpdateDoc->SetDetails(snapshot.GetDetails());
	ptrUpdateDoc->SetHerdId(pOldDoc->GetHerdId());
	ptrUpdateDoc->SetHentId(snapshot.GetHentId());
	ptrUpdateDoc->SetInvoiceId(pOldDoc->GetInvoiceId());
	
	try
	{
		BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
		{
			uint32_t cowId = item->GetCowId();
			CowEntry* pEntry = ptrUpdateDoc->AddCow(cowId);
			pEntry->SetFrom(item->GetEntry());				
		}	

		DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());		
		DocChanges changes = detector.Detect();
		BOOST_FOREACH(DocChange* change,changes)
		{
			if(change->Event() == ChangeEvent_Deleted)
			{
				ICowEntry* pDeletedEntry = change->Old();
				uint32_t cowId = pDeletedEntry->GetCowId();
				CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
				Cow* pDeletedCow = pDeletedCowSO->Object();
				CheckForInStockDuplication(pOldDoc->GetHerdId(),pDeletedCow->GetCowNo());					
			}
		}

		m_DAO.UpdateSellDoc(transaction,ptrUpdateDoc.get());			
	}
	catch(...)
	{
		boost::rethrow_exception(boost::current_exception());
	}

	HerdRegistry* pRegistry = m_Registers.at(pOldDoc->GetHerdId());		
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	pOldDoc->SetDetails(ptrUpdateDoc.get());
	pOldDoc->SetHentId(ptrUpdateDoc->GetHentId());

	std::vector<CowServerObject*> deletedCows;
	DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());		
	DocChanges changes = detector.Detect();
	BOOST_FOREACH(DocChange* change,changes)
	{			
		switch(change->Event())
		{
			case ChangeEvent_Added:
			{
				ICowEntry* pAddedEntry = change->New();
				uint32_t cowId = pAddedEntry->GetCowId();

				CowServerObject* pAddedCowSO = m_Cows.find(cowId);
				Cow* pAddedCow = pAddedCowSO->Object();					
				Lock(pClient,pAddedCowSO);

				CowEntry* pNewEntry = pOldDoc->AddCow(cowId);
				pNewEntry->CopyFrom(pAddedEntry);									

				CowTrail* pTrail = m_Tracker.GetTrail(cowId);
				pTrail->SetExitFootprint(pOldDoc,pNewEntry);

				pRegistry->AddLeavingEvent(pAddedCow,pOldDoc,pNewEntry);
					
				break;
			}
			case ChangeEvent_Deleted:
			{
				ICowEntry* pDeletedEntry = change->Old();
				uint32_t cowId = pDeletedEntry->GetCowId();
				CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
				Cow* pDeletedCow = pDeletedCowSO->Object();
									
				CowTrail* pTrail = m_Tracker.GetTrail(cowId);
				pTrail->EraseExitFootprint();
								
				pRegistry->RemoveLeavingEvent(pDeletedCow,pOldDoc);
				
				pOldDoc->RemoveEntry(cowId);
				deletedCows.push_back(pDeletedCowSO);
				break;
			}
			case ChangeEvent_Updated:
			{
				ICowEntry* pUpdatedEntry = change->New();
				uint32_t cowId = pUpdatedEntry->GetCowId();
				pOldDoc->GetEntry(cowId)->CopyFrom(change->New());
			
				break;
			}
		}
	}

	observerTransaction->OnSellDocUpdated(pOldDoc);	

	BOOST_FOREACH(CowServerObject* pDeletedCowSO,deletedCows)
	{
		Unlock(pClient,pDeletedCowSO);
		observerTransaction->OnCowUnlock(pDeletedCowSO->Object()->GetId());
	}

	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishSellDocUpdate(pOldDoc);
	
}

void SeasonSession::InsertSellDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot)
{
	m_logger->i(L"inserting selldoc: " + snapshot.ToString());
	ValidateSellDocSnapshot(snapshot);
	
	ScopedUpdateState(this);
	
	HerdRegistry* pHerdRegistry = m_Registers.at(snapshot.GetHerdId());
	
	std::auto_ptr<SellDoc> newSellDocDelete;
	
	Transaction transaction = m_DAO.OpenTransaction();
	NewPtrFlushArray<CowServerObject> cowSOs;

	SellDoc* pNewSellDoc = new SellDoc(NULL_ID);
	newSellDocDelete.reset(pNewSellDoc);
	
	const DocDetails& docDetails = snapshot.GetDetails();
	pNewSellDoc->SetDetails(docDetails);	
	pNewSellDoc->SetHerdId(snapshot.GetHerdId());
	pNewSellDoc->SetHentId(snapshot.GetHentId());
	
		
	SellDoc* pInsSellDoc = NULL;	
	CowEntry* pSellDocEntry;
	try
	{
		uint32_t cowId;
		for (size_t i = 0, count = snapshot.GetItemsCount(); i < count; i++)
		{
			const DocCowItem& item = snapshot.GetItem(i);
			const CowDetails& cowDetails = item.GetCow();
			const CowEntryDetails& entryDetails = item.GetEntry();
			cowId = item.GetCowId();
			CowServerObject* pCowSO = FindCow(cowId);			
			if (!pCowSO->OwnsLock(pClient))
			{
				ThrowCowLockNotObtained(cowId);
			}
			
			m_DAO.UpdateCow(transaction,cowId,cowDetails);
			cowSOs.Add(pCowSO);
			pSellDocEntry = pNewSellDoc->AddCow(cowId);
			pSellDocEntry->SetFrom(entryDetails);
		}
		pInsSellDoc = m_DAO.InsertSellDoc(transaction,m_pSeason->GetId(),pNewSellDoc);
	}
	catch(wexception& e)
	{		
		m_logger->d(L"rolling back, cause: " + e.message());
		transaction->Rollback();		
		if(pInsSellDoc)
			delete pInsSellDoc;

		boost::rethrow_exception(boost::current_exception());
	}	
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_SellDocs.add(new SellDocServerObject(pInsSellDoc));
	observerTransaction->OnSellDocInserted(pInsSellDoc);

	pHerdRegistry->AddSellDoc(pInsSellDoc);
		
	uint32_t cowId;
	CowTrail* pTrail;
	ICowEntry* pEntry;
	ICow* pCow;

	for(NewPtrFlushArray<CowServerObject>::iterator cowIt = cowSOs.begin();cowIt != cowSOs.end();cowIt++)
	{
		CowServerObject* pCowSO = *cowIt;
		pCow = pCowSO->Object();
		cowId = pCow->GetId();
		pTrail = m_Tracker.GetTrail(cowId);
		pEntry = pInsSellDoc->GetCow(cowId);
		pTrail->SetExitFootprint(pInsSellDoc,pEntry);	
		pHerdRegistry->AddLeavingEvent(pCow,pInsSellDoc,pEntry);
		observerTransaction->OnCowUpdated(pCow);
	}
	
	transaction->Commit();	
	observerTransaction->Commit();
	m_logger->d(TextUtils::Format(L"inserting new selldoc completed, id=%d", pInsSellDoc->GetId()));

	//m_pChangePublisher->PublishSellDocInsert(pInsSellDoc);


}

void SeasonSession::DeleteSellDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	SellDocServerObject* pDocSO = AcquireSellDoc(docId);
	
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"selldoc(id=%d) delete without acquiring the lock", docId)));

	SellDoc* pDoc = pDocSO->Object();

	ThrowIfThereAreInvoices(pDoc);

	uint32_t herdId = pDoc->GetHerdId();
	HerdRegistry* pHerdRegistry = m_Registers.at(herdId);
	std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));
	Cow* pCow;
	try
	{		
		auto cowIt = cowSOs.begin();
		while(cowIt != cowSOs.end())
		{
			pCow = (*cowIt)->Object();		
			CheckForInStockDuplication(herdId,pCow->GetCowNo());		
			cowIt++;
		}
	}
	catch(...)
	{
		boost::rethrow_exception(boost::current_exception());
	}

	Transaction transaction = m_DAO.OpenTransaction();
	m_DAO.DeleteSellDoc(transaction,docId);
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	uint32_t cowId;
	auto cowIt = cowSOs.begin();
	while(cowIt != cowSOs.end())
	{
		pCow = (*cowIt)->Object();
		cowId = pCow->GetId();
		CowTrail* pTrail = m_Tracker.GetTrail(cowId);
		pTrail->EraseExitFootprint();
		pHerdRegistry->RemoveLeavingEvent(pCow,pDoc);		
		//observerTransaction->OnCowUpdated(pCow);		
		cowIt++;
	}
	pHerdRegistry->RemoveSellDoc(docId);
	observerTransaction->OnSellDocDeleted(docId);
	
	Unlock(pClient,pDocSO);
	m_SellDocs.remove(docId);	
	delete pDocSO;

	std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
													{
														Unlock(pClient,pCowSO);
														observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
													});
	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishSellDocDelete(docId);
}

void SeasonSession::ValidateOutDocSnapshot(const OwnDocSnapshot& snapshot)
{
	const DocDetails& docDetails = snapshot.GetDetails();
	ValidateDocDetails(docDetails);

	if(Motives::GetMotiveDirection(docDetails.GetMotive()) != MotiveDirection_Out)
		ThrowInvalidMotive();	

	ValidateItemCount(snapshot);
}

OutDocServerObject* SeasonSession::FindOutDoc(uint32_t docId)
{
	return m_OutDocs.find(docId);
}

OutDocServerObject* SeasonSession::AcquireOutDoc(uint32_t docId)
{
	OutDocServerObject* pOutDocSO = FindOutDoc(docId);
	if(!pOutDocSO)
		ThrowDocNotFound(DocKey(DocType_Out,docId));
	return pOutDocSO;
}

void SeasonSession::LockOutDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	OutDocServerObject* pDocSO = AcquireOutDoc(docId);	
	IOutDoc* pOutDoc = pDocSO->Object();
	std::vector<CowServerObject*>cowSOs(GetCowSOsForDoc(pOutDoc));

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	if(!Lock(pClient,pDocSO))
		ThrowDocAlreadyLocked(pOutDoc,pDocSO->GetLockHolder());

	try
	{
		LockCowObjects(pClient,cowSOs);
		std::for_each(cowSOs.begin(),cowSOs.end(),[pClient,&observerTransaction](CowServerObject* pCowSO)
												  {
														observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
												  });
		observerTransaction->OnOutDocLock(docId);
		observerTransaction->Commit();
		return;
	}
	catch(...)
	{
		Unlock(pClient,pDocSO);
		boost::rethrow_exception(boost::current_exception());		
	}
}

void SeasonSession::UnlockOutDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);
		
	OutDocServerObject* pDocSO = AcquireOutDoc(docId);
	if(!pDocSO->OwnsLock(pClient))
		ThrowLockHighjack();
		
	IOutDoc* pDoc = pDocSO->Object();
	std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));		
	CheckForLockHighjack(cowSOs,pClient);
		
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
												{
													Unlock(pClient,pCowSO);
													observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
												});

				
	Unlock(pClient,pDocSO);
	observerTransaction->OnOutDocUnlock(docId);		
	observerTransaction->Commit();
	return;
	
}

void SeasonSession::UpdateOutDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot)
{
	ScopedUpdateState(this);

	ValidateOutDocSnapshot(snapshot);

	uint32_t docId = snapshot.GetDocId();
	
	OutDocServerObject* pDocSO = AcquireOutDoc(docId);
		
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"outdoc(id=%d) update without acquiring the lock", docId)));

	OutDoc* pOldDoc = pDocSO->Object();
	Transaction transaction = m_DAO.OpenTransaction();

	std::set<uint32_t> oldCowIds;
		
	PtrEnumerator<ICowEntry> cowsEnum = pOldDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		oldCowIds.insert((*cowsEnum)->GetCowId());
		cowsEnum.advance();
	}
		
	std::auto_ptr<OutDoc> ptrUpdateDoc(new OutDoc(pOldDoc->GetId()));
	
	ptrUpdateDoc->SetDetails(snapshot.GetDetails());
	ptrUpdateDoc->SetHerdId(pOldDoc->GetHerdId());
	
	try
	{
		BOOST_FOREACH(const DocCowItem* item,snapshot.Items())
		{
			uint32_t cowId = item->GetCowId();
			CowEntry* pEntry = ptrUpdateDoc->AddCow(cowId);
			pEntry->SetFrom(item->GetEntry());				
		}	

		DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());		
		DocChanges changes = detector.Detect();
		BOOST_FOREACH(DocChange* change,changes)
		{
			if(change->Event() == ChangeEvent_Deleted)
			{
				ICowEntry* pDeletedEntry = change->Old();
				uint32_t cowId = pDeletedEntry->GetCowId();
				CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
				Cow* pDeletedCow = pDeletedCowSO->Object();
				CheckForInStockDuplication(pOldDoc->GetHerdId(),pDeletedCow->GetCowNo());					
			}
		}


		m_DAO.UpdateOutDoc(transaction,ptrUpdateDoc.get());			
	}
	catch(...)
	{
		boost::rethrow_exception(boost::current_exception());
	}

	HerdRegistry* pRegistry = m_Registers.at(pOldDoc->GetHerdId());		
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	pOldDoc->SetDetails(ptrUpdateDoc.get());

	std::vector<CowServerObject*> deletedCows;
	DocChangeDetector detector(pOldDoc,ptrUpdateDoc.get());		
	DocChanges changes = detector.Detect();
	BOOST_FOREACH(DocChange* change,changes)
	{			
		switch(change->Event())
		{
			case ChangeEvent_Added:
			{
				ICowEntry* pAddedEntry = change->New();
				uint32_t cowId = pAddedEntry->GetCowId();
								
				CowServerObject* pAddedCowSO = m_Cows.find(cowId);
				Cow* pAddedCow = pAddedCowSO->Object();					
				Lock(pClient,pAddedCowSO);

				CowEntry* pNewEntry = pOldDoc->AddCow(cowId);
				pNewEntry->CopyFrom(pAddedEntry);									

				CowTrail* pTrail = m_Tracker.GetTrail(cowId);
				pTrail->SetExitFootprint(pOldDoc,pNewEntry);

				pRegistry->AddLeavingEvent(pAddedCow,pOldDoc,pNewEntry);
					
				break;
			}
			case ChangeEvent_Deleted:
			{
				ICowEntry* pDeletedEntry = change->Old();
				uint32_t cowId = pDeletedEntry->GetCowId();
				CowServerObject* pDeletedCowSO = m_Cows.find(cowId);
				Cow* pDeletedCow = pDeletedCowSO->Object();
									
				CowTrail* pTrail = m_Tracker.GetTrail(cowId);
				pTrail->EraseExitFootprint();
								
				pRegistry->RemoveLeavingEvent(pDeletedCow,pOldDoc);
				
				pOldDoc->RemoveEntry(cowId);
				deletedCows.push_back(pDeletedCowSO);
				break;
			}
			case ChangeEvent_Updated:
			{
				ICowEntry* pUpdatedEntry = change->New();
				uint32_t cowId = pUpdatedEntry->GetCowId();
				pOldDoc->GetEntry(cowId)->CopyFrom(change->New());
			
				break;
			}
		}
	}

	observerTransaction->OnOutDocUpdated(pOldDoc);	

	BOOST_FOREACH(CowServerObject* pDeletedCowSO,deletedCows)
	{
		Unlock(pClient,pDeletedCowSO);
		observerTransaction->OnCowUnlock(pDeletedCowSO->Object()->GetId());
	}

	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishOutDocUpdate(pOldDoc);
	
}

void SeasonSession::InsertOutDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot)
{
	ValidateOutDocSnapshot(snapshot);
	
	ScopedUpdateState(this);
	
	HerdRegistry* pHerdRegistry = m_Registers.at(snapshot.GetHerdId());
	
	std::auto_ptr<OutDoc> newDocDelete;
	
	Transaction transaction = m_DAO.OpenTransaction();
	NewPtrFlushArray<CowServerObject> cowSOs;

	OutDoc* pNewDoc = new OutDoc(NULL_ID);
	newDocDelete.reset(pNewDoc);
	
	const DocDetails& docDetails = snapshot.GetDetails();
	pNewDoc->SetDetails(docDetails);	
	pNewDoc->SetHerdId(snapshot.GetHerdId());	
		
	OutDoc* pInsertedDoc = NULL;	
	CowEntry* pDocEntry;
	try
	{
		uint32_t cowId;
		for (size_t i = 0, count = snapshot.GetItemsCount(); i < count; i++)
		{
			const DocCowItem& item = snapshot.GetItem(i);
			const CowDetails& cowDetails = item.GetCow();
			const CowEntryDetails& entryDetails = item.GetEntry();
			cowId = item.GetCowId();
			CowServerObject* pCowSO = FindCow(cowId);			
			if(!pCowSO->OwnsLock(pClient))
				ThrowCowLockNotObtained(cowId);
			
			m_DAO.UpdateCow(transaction,cowId,cowDetails);
			cowSOs.Add(pCowSO);
			pDocEntry = pNewDoc->AddCow(cowId);
			pDocEntry->SetFrom(entryDetails);
		}
		pInsertedDoc = m_DAO.InsertOutDoc(transaction,m_pSeason->GetId(),pNewDoc);
	}
	catch(...)
	{
		transaction->Rollback();		
		if(pInsertedDoc)
			delete pInsertedDoc;

		boost::rethrow_exception(boost::current_exception());
	}	
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_OutDocs.add(new OutDocServerObject(pInsertedDoc));
	observerTransaction->OnOutDocInserted(pInsertedDoc);

	pHerdRegistry->AddOutDoc(pInsertedDoc);
		
	uint32_t cowId;
	CowTrail* pTrail;
	ICowEntry* pEntry;
	ICow* pCow;

	for(NewPtrFlushArray<CowServerObject>::iterator cowIt = cowSOs.begin();cowIt != cowSOs.end();cowIt++)
	{
		CowServerObject* pCowSO = *cowIt;
		pCow = pCowSO->Object();
		cowId = pCow->GetId();
		pTrail = m_Tracker.GetTrail(cowId);
		pEntry = pInsertedDoc->GetCow(cowId);
		pTrail->SetExitFootprint(pInsertedDoc,pEntry);		

		pHerdRegistry->AddLeavingEvent(pCow,pInsertedDoc,pEntry);
		observerTransaction->OnCowUpdated(pCow);
	}
	
	transaction->Commit();	
	observerTransaction->Commit();

	//m_pChangePublisher->PublishOutDocInsert(pInsertedDoc);

}

void SeasonSession::DeleteOutDoc(ClientSeasonSession* pClient,uint32_t docId)
{
	ScopedUpdateState(this);

	OutDocServerObject* pDocSO = AcquireOutDoc(docId);
	
	if(!pDocSO->OwnsLock(pClient))
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"outdoc(id=%d) delete without acquiring the lock", docId)));

	OutDoc* pDoc = pDocSO->Object();

	ThrowIfThereAreInvoices(pDoc);

	uint32_t herdId = pDoc->GetHerdId();
	HerdRegistry* pHerdRegistry = m_Registers.at(herdId);
	std::vector<CowServerObject*> cowSOs(GetCowSOsForDoc(pDoc));
	Cow* pCow;
	try
	{		
		auto cowIt = cowSOs.begin();
		while(cowIt != cowSOs.end())
		{
			pCow = (*cowIt)->Object();		
			CheckForInStockDuplication(herdId,pCow->GetCowNo());		
			cowIt++;
		}
	}
	catch(...)
	{		
		boost::rethrow_exception(boost::current_exception());
	}

	Transaction transaction = m_DAO.OpenTransaction();
	m_DAO.DeleteOutDoc(transaction,docId);
	
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	uint32_t cowId;
	auto cowIt = cowSOs.begin();
	while(cowIt != cowSOs.end())
	{
		pCow = (*cowIt)->Object();
		cowId = pCow->GetId();
		CowTrail* pTrail = m_Tracker.GetTrail(cowId);
		pTrail->EraseExitFootprint();
		pHerdRegistry->RemoveLeavingEvent(pCow,pDoc);		
		cowIt++;
	}

	pHerdRegistry->RemoveOutDoc(docId);
	observerTransaction->OnOutDocDeleted(docId);
	
	Unlock(pClient,pDocSO);
	m_OutDocs.remove(docId);	
	delete pDocSO;

	std::for_each(cowSOs.begin(),cowSOs.end(),[this,pClient,observerTransaction](CowServerObject* pCowSO)
													{
														Unlock(pClient,pCowSO);
														observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
													});
	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishOutDocDelete(docId);
}

void SeasonSession::CheckIfCowNoChangeIsAllowed(uint32_t cowId,const CowNo& newCowNo)
{
	CowIds ids = m_CowNoReference.GetCowsWithCowNo(newCowNo);
}

void SeasonSession::ValidateInvoiceDetails(const InvoiceDetails& details)
{

}

void SeasonSession::ValidateInvoiceSnapshot(const InvoiceSnapshot& snapshot)
{
	Decimal totalPrice(0L);
	ValidateInvoiceDetails(snapshot.GetDetails());
	BOOST_FOREACH(const InvoiceCowItem* item,snapshot.Items())
	{
		totalPrice += item->GetEntry().GetPrice();
	}

	if (totalPrice <= Decimal(0L))
	{
		BOOST_THROW_EXCEPTION(SessionException(L"total_price_not_gt_0"));
	}
	
}

void SeasonSession::ThrowInvoiceAlreadyLocked(IInvoice* pInvoice,ClientSeasonSession* pSession)
{
	ClientSession* pClient = pSession->GetClientSession();
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"invoice_already_locked")
									 (L"user_id",pClient->GetUserId())
									 .str()));
}

void SeasonSession::ThrowDocAlreadyLocked(IDoc* pDoc,ClientSeasonSession* pSession)
{
	ClientSession* pClient = pSession->GetClientSession();
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"doc_already_locked")
									 (L"user_id",pClient->GetUserId())
									 .str()));
}

std::vector<CowServerObject*> SeasonSession::GetCowSOsForInvoice(const IInvoice* pInvoice)
{
	ICowInvoiceEntry* pEntry;
	CowServerObject* pCowSO;
	uint32_t cowId;
	PtrEnumerator<ICowInvoiceEntry> cowsEnum = pInvoice->EnumEntries();
	std::vector<CowServerObject*> cowCOs;
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();
		pCowSO = FindCow(cowId);
		if(!pCowSO)
		{			
			ThrowCowNotFound(cowId);
		}
		cowCOs.push_back(pCowSO);
		cowsEnum.advance();
	}

	return std::move(cowCOs);
}


void SeasonSession::LockBuyInvoice(ClientSeasonSession* pClient,uint32_t invoiceId)
{
	ScopedUpdateState(this);

	Memorandum fallback;
	BuyInvoiceServerObject* pBuyInvoiceSO = AcquireBuyInvoice(invoiceId);
	IBuyInvoice* pBuyInvoice = pBuyInvoiceSO->Object();
	
	std::vector<DocKey> keys = m_BuyInvoiceRelations.GetInvoiceRelations(invoiceId);
	
	if(!Lock(pClient,pBuyInvoiceSO))
		ThrowInvoiceAlreadyLocked(pBuyInvoice,pBuyInvoiceSO->GetLockHolder());

	fallback.Add(std::bind(&SeasonSession::Unlock<BuyInvoiceServerObject>,this,pClient,pBuyInvoiceSO));

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();	
	
	try
	{
		//std::vector<CowServerObject*> cowSOs;
		/*BOOST_FOREACH(DocKey key,keys)
		{
			uint32_t docId = key.GetDocNo();
			switch(key.GetDocType())
			{
				case DocType_Buy:
				{					
					BuyDocServerObject* pDocSO = FindBuyDoc(docId);
											
					if(!Lock(pClient,pDocSO))
						ThrowDocAlreadyLocked(pDocSO->Object(),pDocSO->GetLockHolder());
			
					fallback.Add(std::bind(&SeasonSession::Unlock<BuyDocServerObject>,this,pClient,pDocSO));
					observerTransaction->OnBuyDocLock(docId);	

					GetCowSOsForDoc(cowSOs,pDocSO->Object());
					break;
				}
				case DocType_In:
				{
					InDocServerObject* pDocSO = FindInDoc(docId);
											
					if(!Lock(pClient,pDocSO))
						ThrowDocAlreadyLocked(pDocSO->Object(),pDocSO->GetLockHolder());
			
					fallback.Add(std::bind(&SeasonSession::Unlock<InDocServerObject>,this,pClient,pDocSO));
					observerTransaction->OnInDocLock(docId);

					GetCowSOsForDoc(cowSOs,pDocSO->Object());
					break;
				}
			}
		}
*/
		std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pBuyInvoice);
		LockCowObjects(pClient,cowSOs);
			
		std::for_each(cowSOs.begin(),cowSOs.end(),[&](CowServerObject* pCowSO)
												{
													fallback.Add(std::bind(&SeasonSession::Unlock<CowServerObject>,this,pClient,pCowSO));
													observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
												});

		observerTransaction->OnBuyInvoiceLocked(invoiceId);	

	}
	catch(...)
	{		
		fallback.Do();		
		boost::rethrow_exception(boost::current_exception());		
	}	
	observerTransaction->Commit();
}

void SeasonSession::UnlockBuyInvoice(ClientSeasonSession* pClient,uint32_t invoiceId)
{
	ScopedUpdateState(this);
		
	BuyInvoiceServerObject* pBuyInvoiceSO = AcquireBuyInvoice(invoiceId);
	BuyInvoice* pBuyInvoice = pBuyInvoiceSO->Object();
	
	if(!pBuyInvoiceSO->OwnsLock(pClient))
			ThrowLockHighjack();

	//std::vector<DocKey> keys(m_BuyInvoiceRelations.GetInvoiceRelations(invoiceId));
		
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();	

	/*std::function<void (BuyDocServerObject*)> buyDocUnlockAction =	[this,pClient,observerTransaction](BuyDocServerObject* pDocSO)
																	{
																		Unlock(pClient,pDocSO);
																		observerTransaction->OnBuyDocUnlock(pDocSO->Object()->GetId());
																	};

	std::function<void (InDocServerObject*)> inDocUnlockAction =	[this,pClient,observerTransaction](InDocServerObject* pDocSO)
																	{
																		Unlock(pClient,pDocSO);
																		observerTransaction->OnInDocUnlock(pDocSO->Object()->GetId());
																	};*/

	//std::vector<CowServerObject*> cowSOs;
	Memorandum unlockActions;
	/*BOOST_FOREACH(DocKey key,keys)
	{
		uint32_t docId = key.GetDocNo();
		switch(key.GetDocType())
		{
			case DocType_Buy:
			{
				BuyDocServerObject* pDocSO = FindBuyDoc(docId);		
				if(!pDocSO->OwnsLock(pClient)) 	ThrowLockHighjack();	
				unlockActions.Add(std::bind(buyDocUnlockAction,pDocSO));
				GetCowSOsForDoc(cowSOs,pDocSO->Object());
				break;
			}
			case DocType_In:
			{
				InDocServerObject* pDocSO = FindInDoc(docId);		
				if(!pDocSO->OwnsLock(pClient)) 	ThrowLockHighjack();			
				unlockActions.Add(std::bind(inDocUnlockAction,pDocSO));
				GetCowSOsForDoc(cowSOs,pDocSO->Object());
				break;
			}
		}				
		
	}*/
	std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pBuyInvoice);
	CheckForLockHighjack(cowSOs,pClient);

	std::for_each(cowSOs.begin(),cowSOs.end(),	[this,pClient,observerTransaction](CowServerObject* pCowSO)
												{
													Unlock(pClient,pCowSO);
													observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
												});


	unlockActions.Do();
					
	Unlock(pClient,pBuyInvoiceSO);
	observerTransaction->OnBuyInvoiceUnlocked(invoiceId);		
	observerTransaction->Commit();
	return;	
}

void SeasonSession::ThrowInvoiceNotFound(const InvoiceKey& key)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"invoice key=%c%d not found in season", key.GetDirection(), key.GetInvoiceId())));
}

BuyInvoiceServerObject* SeasonSession::FindBuyInvoice(uint32_t invoiceId)
{
	return m_BuyInvoices.find(invoiceId);
}

BuyInvoiceServerObject* SeasonSession::AcquireBuyInvoice(uint32_t invoiceId)
{
	BuyInvoiceServerObject* pBuyInvoiceSO = FindBuyInvoice(invoiceId);
	if(!pBuyInvoiceSO)
		ThrowInvoiceNotFound(InvoiceKey(InvoiceDirection_Buy,invoiceId));
	return pBuyInvoiceSO;
}

void SeasonSession::InsertBuyDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState(this);
	
	Transaction transaction = m_DAO.OpenTransaction();
	
	BuyInvoice* pInvoice = new BuyInvoice(NULL_ID,InvoiceScope_Docs);
	std::auto_ptr<BuyInvoice> newInvoiceDelete(pInvoice);

	uint32_t invoiceNo = ObtainNextBuyInvoiceNo(transaction);

	pInvoice->SetInvoiceNo(invoiceNo);
	pInvoice->SetFrom(snapshot.GetDetails());
	pInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());	
	pInvoice->SetInvoiceType(snapshot.GetInvoiceType());
	
	std::set<uint32_t> docIds;
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();
	BuyInvoice* pInsertedInvoice = NULL;
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetClassId(entryDetails.GetClassId());
		pEntry->SetPrice(entryDetails.GetPrice());
		pEntry->SetWeight(entryDetails.GetWeight());

		docIds.insert(pItem->GetDocId());
	}

	BOOST_FOREACH(const IInvoiceDeduction* pDeductionItem, snapshot.Deductions())
	{
		pInvoice->AddDeduction(pDeductionItem);
	}

	std::vector<BuyDocServerObject*> docSOs;
	
	try
	{
		pInsertedInvoice = m_DAO.InsertBuyInvoice(transaction,m_pSeason->GetId(),pInvoice);
	
		BOOST_FOREACH(uint32_t docId,docIds)
		{
			m_DAO.UpdateBuyDocWithInvoice(transaction,docId,pInsertedInvoice->GetId());
			docSOs.push_back(AcquireBuyDoc(docId));
		}

	}
	catch(...)
	{
		transaction->Rollback();		
		if(pInsertedInvoice)
			delete pInsertedInvoice;

		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_BuyInvoices.add(new BuyInvoiceServerObject(pInsertedInvoice));
	observerTransaction->OnBuyInvoiceInserted(pInsertedInvoice);

	uint32_t invoiceId = pInsertedInvoice->GetId();
	BOOST_FOREACH(BuyDocServerObject* pDocSO,docSOs)
	{
		BuyDoc* pBuyDoc = pDocSO->Object();
		pBuyDoc->SetInvoiceId(invoiceId);
		m_BuyInvoiceRelations.AddRelationship(invoiceId,DocKey(DocType_Buy,pBuyDoc->GetId()));
		observerTransaction->OnBuyDocUpdated(pBuyDoc);
		std::vector<CowServerObject*> cowSOs = GetCowSOsForDoc(pBuyDoc);
		BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
		{
			observerTransaction->OnCowUpdated(pCowSO->Object());
		}
	}

	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishBuyInvoiceInsert(pInsertedInvoice);
}

void SeasonSession::InsertBuyCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState state(this);

	Transaction transaction = m_DAO.OpenTransaction();
	
	BuyInvoice* pInvoice = new BuyInvoice(NULL_ID,InvoiceScope_Cows);
	std::auto_ptr<BuyInvoice> newInvoiceDelete(pInvoice);

	uint32_t invoiceNo = ObtainNextBuyInvoiceNo(transaction);

	pInvoice->SetInvoiceNo(invoiceNo);
	pInvoice->SetFrom(snapshot.GetDetails());
	pInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());	
	pInvoice->SetInvoiceType(snapshot.GetInvoiceType());
	
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();

	uint32_t docId = NULL_ID;
	
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetClassId(entryDetails.GetClassId());
		pEntry->SetPrice(entryDetails.GetPrice());
		pEntry->SetWeight(entryDetails.GetWeight());

		docId = pItem->GetDocId();
	}	

	BOOST_FOREACH(const IInvoiceDeduction* pDeductionItem, snapshot.Deductions())
	{
		pInvoice->AddDeduction(pDeductionItem);
	}
	
	std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pInvoice);
	BuyInvoice* pInsertedInvoice = NULL;
	try
	{
		pInsertedInvoice = m_DAO.InsertBuyInvoice(transaction,m_pSeason->GetId(),pInvoice);
	
		BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
		{
			m_DAO.UpdateCowWithBuyInvoice(transaction,pCowSO->Object()->GetId(),pInsertedInvoice->GetId());			
		}

	}
	catch(...)
	{
		transaction->Rollback();		
		if(pInsertedInvoice)
			delete pInsertedInvoice;

		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_BuyInvoices.add(new BuyInvoiceServerObject(pInsertedInvoice));
	observerTransaction->OnBuyInvoiceInserted(pInsertedInvoice);

	uint32_t invoiceId = pInsertedInvoice->GetId();
	BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
	{
		Cow* pCow = pCowSO->Object();
		pCow->SetBuyInvoiceId(invoiceId);
		observerTransaction->OnCowUpdated(pCow);
	}

	m_BuyInvoiceRelations.AddRelationship(invoiceId,DocKey(DocType_In,docId));

	transaction->Commit();
	observerTransaction->Commit();

	//m_pChangePublisher->PublishBuyInvoiceInsert(pInsertedInvoice);
}

std::set<uint32_t> SeasonSession::GetEntryDocIdsForInvoice(const IInvoice* pInvoice)
{
	std::set<uint32_t> docIds;
	ICowInvoiceEntry* pEntry;
	ICowTrail* pTrail;
	PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		pTrail = m_Tracker.GetTrail(pEntry->GetCowId());		
		docIds.insert(pTrail->GetEntry()->GetDoc()->GetId());
		entryEnum.advance();
	}

	return std::move(docIds);
}

std::set<uint32_t> SeasonSession::GetExitDocIdsForInvoice(const IInvoice* pInvoice)
{
	std::set<uint32_t> docIds;
	ICowInvoiceEntry* pEntry;
	ICowTrail* pTrail;
	const ICowFootprint* pFootprint;
	PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
	while (entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		pTrail = m_Tracker.GetTrail(pEntry->GetCowId());
		pFootprint = pTrail->GetExit();
		if (pFootprint)
			docIds.insert(pFootprint->GetDoc()->GetId());
		entryEnum.advance();
	}

	return std::move(docIds);
}


std::vector<BuyDocServerObject*> SeasonSession::AcquireBuyDocSOs(IdRange ids)
{
	std::vector<BuyDocServerObject*> docs;
	BOOST_FOREACH(uint32_t docId,ids)
	{
		docs.push_back(AcquireBuyDoc(docId));
	}
	return std::move(docs);
}

std::vector<SellDocServerObject*> SeasonSession::AcquireSellDocSOs(IdRange ids)
{
	std::vector<SellDocServerObject*> docs;
	BOOST_FOREACH(uint32_t docId,ids)
	{
		docs.push_back(AcquireSellDoc(docId));
	}
	return std::move(docs);
}



void SeasonSession::UpdateBuyDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState(this);
	
	BuyInvoiceServerObject* pOldInvoiceSO = AcquireBuyInvoice(snapshot.GetInvoiceId());
	BuyInvoice* pOldInvoice = pOldInvoiceSO->Object();
		
	BuyInvoice* pReferenceInvoice = new BuyInvoice(pOldInvoice->GetId(),InvoiceScope_Docs);
	std::auto_ptr<BuyInvoice> _guard(pReferenceInvoice);

	pReferenceInvoice->SetFrom(snapshot.GetDetails());
	pReferenceInvoice->SetInvoiceNo(pOldInvoice->GetInvoiceNo());
	pReferenceInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	pReferenceInvoice->SetInvoiceType(snapshot.GetInvoiceType());
			
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pReferenceInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetFrom(entryDetails);		
	}

	BOOST_FOREACH(const IInvoiceDeduction* pDeductionItem, snapshot.Deductions())
	{
		pReferenceInvoice->AddDeduction(pDeductionItem);
	}

	std::set<uint32_t> oldInvoiceDocIds = GetEntryDocIdsForInvoice(pOldInvoice);
	std::set<uint32_t> refInvoiceDocIds = GetEntryDocIdsForInvoice(pReferenceInvoice);

	std::vector<BuyDocServerObject*> oldInvoiceDocSOs = AcquireBuyDocSOs(IdRange(oldInvoiceDocIds));
	std::vector<BuyDocServerObject*> newInvoiceDocSOs = AcquireBuyDocSOs(IdRange(refInvoiceDocIds));
	
	Diffrence<BuyDocServerObject*> docDiffrence(oldInvoiceDocSOs,newInvoiceDocSOs);

	Diffrence<BuyDocServerObject*>::Range addedDocs = docDiffrence.Added();
	Diffrence<BuyDocServerObject*>::Range removedDocs = docDiffrence.Removed();	
	Diffrence<BuyDocServerObject*>::Range commonDocs = docDiffrence.Common();	

	Transaction transaction = m_DAO.OpenTransaction();
	try
	{
		BOOST_FOREACH(BuyDoc* pBuyDoc,(addedDocs | boost::adaptors::transformed(ServerObjectPeeler<BuyDocServerObject>())))
		{
			m_DAO.UpdateBuyDocWithInvoice(transaction,pBuyDoc->GetId(),pOldInvoice->GetId());			
		}

		BOOST_FOREACH(BuyDoc* pBuyDoc,(removedDocs | boost::adaptors::transformed(ServerObjectPeeler<BuyDocServerObject>())))
		{
			m_DAO.UpdateBuyDocWithInvoice(transaction,pBuyDoc->GetId(),NULL_ID);			
		}

		m_DAO.UpdateBuyInvoice(transaction,pReferenceInvoice);

	}
	catch(...)
	{
		transaction->Rollback();
		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	pOldInvoice->SetFrom(snapshot.GetDetails());
	pOldInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	
	InvoiceChangeDetector changeDetector(pOldInvoice,pReferenceInvoice);
	InvoiceChanges changes = changeDetector.Detect();

	BOOST_FOREACH(InvoiceChange* change,changes)
	{
		switch(change->Event())
		{
		case ChangeEvent_Added:
			pOldInvoice->AddEntry(new CowInvoiceEntry(change->New()));
			break;

		case ChangeEvent_Deleted:
			pOldInvoice->RemoveEntry(change->Old()->GetCowId());
			break;

		case ChangeEvent_Updated:
			pOldInvoice->GetEntry(change->Old()->GetCowId())->CopyFrom(change->New());
			break;
		}
	}

	pOldInvoice->RemoveAllDeductions();

	PtrEnumerator<IInvoiceDeduction> deductionEnum = pReferenceInvoice->EnumDeductions();
	while (deductionEnum.hasNext())
	{
		pOldInvoice->AddDeduction(*deductionEnum);
		deductionEnum.advance();
	}

	observerTransaction->OnBuyInvoiceUpdated(pOldInvoice);

	uint32_t invoiceId = pOldInvoice->GetId();
	BOOST_FOREACH(BuyDoc* pBuyDoc,(addedDocs | boost::adaptors::transformed(ServerObjectPeeler<BuyDocServerObject>())))
	{
		pBuyDoc->SetInvoiceId(invoiceId);
		observerTransaction->OnBuyDocUpdated(pBuyDoc);
		m_BuyInvoiceRelations.AddRelationship(invoiceId,DocKey(DocType_Buy,pBuyDoc->GetId()));		
	}

	BOOST_FOREACH(BuyDoc* pBuyDoc,(removedDocs | boost::adaptors::transformed(ServerObjectPeeler<BuyDocServerObject>())))
	{
		pBuyDoc->SetInvoiceId(NULL_ID);
		observerTransaction->OnBuyDocUpdated(pBuyDoc);
		m_BuyInvoiceRelations.RemoveRelationship(invoiceId,DocKey(DocType_Buy,pBuyDoc->GetId()));		
	}

	std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pOldInvoice);

	BOOST_FOREACH(Cow* pCow,(cowSOs | boost::adaptors::transformed(ServerObjectPeeler<CowServerObject>())))
	{
		observerTransaction->OnCowUpdated(pCow);
	}
		
	transaction->Commit();
	observerTransaction->Commit();
	//m_pChangePublisher->PublishBuyInvoiceUpdate(pOldInvoice);
}

void SeasonSession::UpdateBuyCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState(this);
	
	BuyInvoiceServerObject* pOldInvoiceSO = AcquireBuyInvoice(snapshot.GetInvoiceId());
	BuyInvoice* pOldInvoice = pOldInvoiceSO->Object();
		
	BuyInvoice* pReferenceInvoice = new BuyInvoice(pOldInvoice->GetId(),InvoiceScope_Docs);
	std::auto_ptr<BuyInvoice> _guard(pReferenceInvoice);

	pReferenceInvoice->SetFrom(snapshot.GetDetails());
	pReferenceInvoice->SetInvoiceNo(pOldInvoice->GetInvoiceNo());
	pReferenceInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	pReferenceInvoice->SetInvoiceType(snapshot.GetInvoiceType());
			
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pReferenceInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetFrom(entryDetails);		
	}

	BOOST_FOREACH(const IInvoiceDeduction* pDeductionItem, snapshot.Deductions())
	{
		pReferenceInvoice->AddDeduction(pDeductionItem);
	}

	InvoiceChangeDetector changeDetector(pOldInvoice,pReferenceInvoice);
	InvoiceChanges changes = changeDetector.Detect();
	uint32_t invoiceId = pOldInvoice->GetId();

	Transaction transaction = m_DAO.OpenTransaction();
	try
	{		
		BOOST_FOREACH(InvoiceChange* change,changes)
		{
			switch(change->Event())
			{
			case ChangeEvent_Added:
				m_DAO.UpdateCowWithBuyInvoice(transaction,change->New()->GetCowId(),invoiceId);
				break;
			case ChangeEvent_Deleted:
				m_DAO.UpdateCowWithBuyInvoice(transaction,change->Old()->GetCowId(),NULL_ID);								
				break;
			}
		}
		m_DAO.UpdateBuyInvoice(transaction,pReferenceInvoice);
	}
	catch(...)
	{
		transaction->Rollback();
		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	pOldInvoice->SetFrom(snapshot.GetDetails());
	pOldInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	pOldInvoice->RemoveAllDeductions();

	PtrEnumerator<IInvoiceDeduction> deductionEnum = pReferenceInvoice->EnumDeductions();
	while (deductionEnum.hasNext())
	{
		pOldInvoice->AddDeduction(*deductionEnum);
		deductionEnum.advance();
	}


	CowServerObject* pCowSO;
	Cow* pCow;
	
	Streak* pInvoiceUpdate = observerTransaction->StartStreak();
	Streak* pCowsUpdate = observerTransaction->StartStreak();

	BOOST_FOREACH(InvoiceChange* change,changes)
	{
		pCowSO = AcquireCow(change->GetCowId());
		pCow = pCowSO->Object();

		switch(change->Event())
		{
		case ChangeEvent_Added:
			pOldInvoice->AddEntry(new CowInvoiceEntry(change->New()));			
			pCow->SetBuyInvoiceId(invoiceId);			
			break;

		case ChangeEvent_Deleted:			
			pOldInvoice->RemoveEntry(change->Old()->GetCowId());			
			pCow->SetBuyInvoiceId(NULL_ID);			
			break;

		case ChangeEvent_Updated:
			pOldInvoice->GetEntry(change->Old()->GetCowId())->CopyFrom(change->New());			
			break;
		}
		pCowsUpdate->OnCowUpdated(pCow);
	}

	pInvoiceUpdate->OnBuyInvoiceUpdated(pOldInvoice);	 	
	transaction->Commit();
	observerTransaction->Commit();
	//m_pChangePublisher->PublishBuyInvoiceUpdate(pOldInvoice);
}

void SeasonSession::ThrowIfInvoiceIsPaid(IInvoice* pInvoice)
{
	if (!pInvoice->GetPaidDate().IsNull())
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"invoice_is_paid")(L"invoice_id", pInvoice->GetId())(L"paid_dt", pInvoice->GetPaidDate().GetDayDate()).str()));
}

void SeasonSession::DeleteBuyInvoice(ClientSeasonSession* pClient,uint32_t invoiceId)
{
	ScopedUpdateState(this);

	BuyInvoiceServerObject* pBuyInvoiceSO = AcquireBuyInvoice(invoiceId);

	if(!pBuyInvoiceSO->OwnsLock(pClient))
		ThrowInvoiceLockNotObtained(invoiceId);

	BuyInvoice* pBuyInvoice =  pBuyInvoiceSO->Object();
	
	ThrowIfInvoiceIsPaid(pBuyInvoice);

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	Transaction transaction = m_DAO.OpenTransaction();

	switch(pBuyInvoice->GetInvoiceScope())
	{
		case InvoiceScope_Docs:
		{
			std::vector<DocKey> keys = m_BuyInvoiceRelations.GetInvoiceRelationsOf(invoiceId,DocType_Buy);
			std::vector<BuyDocServerObject*> docSOs;
			BOOST_FOREACH(DocKey key,keys)
			{				
				docSOs.push_back(FindBuyDoc(key.GetDocNo()));
				m_DAO.UpdateBuyDocWithInvoice(transaction,key.GetDocNo(),NULL_ID);				
			}			
			m_DAO.DeleteBuyInvoice(transaction,invoiceId);			

			BOOST_FOREACH(BuyDocServerObject* pDocSO,docSOs)
			{				
				BuyDoc* pDoc = pDocSO->Object();
				pDoc->SetInvoiceId(NULL_ID);
				observerTransaction->OnBuyDocUpdated(pDoc);		

				std::vector<CowServerObject*> cowSOs = GetCowSOsForDoc(pDoc);
				BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
				{
					Cow* pCow = pCowSO->Object();
					observerTransaction->OnCowUpdated(pCow);		
					Unlock(pClient,pCowSO);
					observerTransaction->OnCowUnlock(pCow->GetId());		
				}
				Unlock(pClient,pDocSO);
				observerTransaction->OnBuyDocUnlock(pDoc->GetId());						
			}
			m_BuyInvoiceRelations.RemoveRelations(invoiceId,keys);

			break;
		}

		case InvoiceScope_Cows:
		{			
			std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pBuyInvoice);
			DocKey key = m_BuyInvoiceRelations.GetInvoiceRelationship(invoiceId);
			InDocServerObject* pDocSO = AcquireInDoc(key.GetDocNo());

			BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
			{					
				m_DAO.UpdateCowWithBuyInvoice(transaction,pCowSO->Object()->GetId(),NULL_ID);				
			}			
			m_DAO.DeleteBuyInvoice(transaction,invoiceId);			

			BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
			{	
				Cow* pCow = pCowSO->Object();
				pCow->SetBuyInvoiceId(NULL_ID);
				observerTransaction->OnCowUpdated(pCowSO->Object());						
			}

			cowSOs = GetCowSOsForDoc(pDocSO->Object());
			BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
			{
				Unlock(pClient,pCowSO);
				observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());			
			}
					
			Unlock(pClient,pDocSO);
			observerTransaction->OnInDocUnlock(pDocSO->Object()->GetId());			
			m_BuyInvoiceRelations.RemoveRelationship(invoiceId,key);
			break;
		}
		default:
			ThrowFatalError( __WFILE__,__LINE__,L"invalid scope");
	}

	observerTransaction->OnBuyInvoiceDelete(invoiceId);
	Unlock(pClient,pBuyInvoiceSO);
	m_BuyInvoices.remove(invoiceId);
	delete pBuyInvoiceSO;

	observerTransaction->Commit();
	transaction->Commit();

	//m_pChangePublisher->PublishBuyInvoiceDelete(invoiceId);
}

SellInvoiceServerObject* SeasonSession::FindSellInvoice(uint32_t invoiceId)
{
	return m_SellInvoices.find(invoiceId);
}

SellInvoiceServerObject* SeasonSession::AcquireSellInvoice(uint32_t invoiceId)
{
	SellInvoiceServerObject* pSellInvoiceSO = FindSellInvoice(invoiceId);
	if(!pSellInvoiceSO)
		ThrowInvoiceNotFound(InvoiceKey(InvoiceDirection_Sell,invoiceId));
	return pSellInvoiceSO;
}

void SeasonSession::LockSellInvoice(ClientSeasonSession* pClient,uint32_t invoiceId)
{
	ScopedUpdateState(this);

	Memorandum fallback;
	SellInvoiceServerObject* pSellInvoiceSO = AcquireSellInvoice(invoiceId);
	ISellInvoice* pSellInvoice = pSellInvoiceSO->Object();
	
	//std::vector<DocKey> keys = m_SellInvoiceRelations.GetInvoiceRelations(invoiceId);
	
	if(!Lock(pClient,pSellInvoiceSO))
		ThrowInvoiceAlreadyLocked(pSellInvoice,pSellInvoiceSO->GetLockHolder());

	fallback.Add(std::bind(&SeasonSession::Unlock<SellInvoiceServerObject>,this,pClient,pSellInvoiceSO));

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();	
	
	try
	{
		/*std::vector<CowServerObject*> cowSOs;
		BOOST_FOREACH(DocKey key,keys)
		{
			uint32_t docId = key.GetDocNo();
			switch(key.GetDocType())
			{
				case DocType_Sell:
				{					
					SellDocServerObject* pDocSO = FindSellDoc(docId);
											
					if(!Lock(pClient,pDocSO))
						ThrowDocAlreadyLocked(pDocSO->Object(),pDocSO->GetLockHolder());
			
					fallback.Add(std::bind(&SeasonSession::Unlock<SellDocServerObject>,this,pClient,pDocSO));
					observerTransaction->OnSellDocLock(docId);	

					GetCowSOsForDoc(cowSOs,pDocSO->Object());
					break;
				}
				case DocType_Out:
				{
					OutDocServerObject* pDocSO = FindOutDoc(docId);
											
					if(!Lock(pClient,pDocSO))
						ThrowDocAlreadyLocked(pDocSO->Object(),pDocSO->GetLockHolder());
			
					fallback.Add(std::bind(&SeasonSession::Unlock<OutDocServerObject>,this,pClient,pDocSO));
					observerTransaction->OnOutDocLock(docId);

					GetCowSOsForDoc(cowSOs,pDocSO->Object());
					break;
				}
			}
		}*/

		std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pSellInvoice);
		LockCowObjects(pClient,cowSOs);
			
		std::for_each(cowSOs.begin(),cowSOs.end(),[&](CowServerObject* pCowSO)
												{
													fallback.Add(std::bind(&SeasonSession::Unlock<CowServerObject>,this,pClient,pCowSO));
													observerTransaction->OnCowLock(pCowSO->Object()->GetId());																			
												});

		observerTransaction->OnSellInvoiceLocked(invoiceId);	

	}
	catch(...)
	{		
		fallback.Do();		
		boost::rethrow_exception(boost::current_exception());		
	}	
	observerTransaction->Commit();
}

void SeasonSession::UnlockSellInvoice(ClientSeasonSession* pClient,uint32_t invoiceId)
{
	ScopedUpdateState(this);
		
	SellInvoiceServerObject* pSellInvoiceSO = AcquireSellInvoice(invoiceId);
	SellInvoice* pSellInvoice = pSellInvoiceSO->Object();
	
	if(!pSellInvoiceSO->OwnsLock(pClient))
			ThrowLockHighjack();

	//std::vector<DocKey> keys(m_SellInvoiceRelations.GetInvoiceRelations(invoiceId));
		
	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();	
		
	//std::vector<CowServerObject*> cowSOs;
	Memorandum unlockActions;
	/*BOOST_FOREACH(DocKey key,keys)
	{
		uint32_t docId = key.GetDocNo();
		switch(key.GetDocType())
		{
			case DocType_Sell:
			{
				SellDocServerObject* pDocSO = FindSellDoc(docId);		
				if(!pDocSO->OwnsLock(pClient)) 	ThrowLockHighjack();	
				GetCowSOsForDoc(cowSOs,pDocSO->Object());

				unlockActions.Add(std::bind(&SeasonSession::Unlock<SellDocServerObject>,this,pClient,pDocSO));				
				unlockActions.Add(std::bind(&ObserverTransaction::OnSellDocUnlock,observerTransaction,pDocSO->Object()->GetId()));				
				break;
			}
			case DocType_Out:
			{
				OutDocServerObject* pDocSO = FindOutDoc(docId);		
				if(!pDocSO->OwnsLock(pClient)) 	ThrowLockHighjack();			
				GetCowSOsForDoc(cowSOs,pDocSO->Object());

				unlockActions.Add(std::bind(&SeasonSession::Unlock<OutDocServerObject>,this,pClient,pDocSO));				
				unlockActions.Add(std::bind(&ObserverTransaction::OnOutDocUnlock,observerTransaction,pDocSO->Object()->GetId()));								
				break;
			}
		}	
	}*/
	
	std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pSellInvoice);
	CheckForLockHighjack(cowSOs,pClient);

	std::for_each(cowSOs.begin(),cowSOs.end(),	[this,pClient,observerTransaction](CowServerObject* pCowSO)
												{
													Unlock(pClient,pCowSO);
													observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());
												});


	unlockActions.Do();
					
	Unlock(pClient,pSellInvoiceSO);
	observerTransaction->OnSellInvoiceUnlocked(invoiceId);		
	observerTransaction->Commit();
	return;	
}

void SeasonSession::InsertSellDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState(this);
	
	Transaction transaction = m_DAO.OpenTransaction();
	
	SellInvoice* pInvoice = new SellInvoice(NULL_ID,InvoiceScope_Docs);
	std::auto_ptr<SellInvoice> newInvoiceDelete(pInvoice);

	uint32_t invoiceNo = ObtainNextSellInvoiceNo(transaction);

	pInvoice->SetInvoiceNo(invoiceNo);
	pInvoice->SetFrom(snapshot.GetDetails());
	pInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());	
	pInvoice->SetInvoiceType(snapshot.GetInvoiceType());
	
	std::set<uint32_t> docIds;
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();
	SellInvoice* pInsertedInvoice = NULL;
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetClassId(entryDetails.GetClassId());
		pEntry->SetPrice(entryDetails.GetPrice());
		pEntry->SetWeight(entryDetails.GetWeight());

		docIds.insert(pItem->GetDocId());
	}

	BOOST_FOREACH(const IInvoiceDeduction* pDeductionItem, snapshot.Deductions())
	{
		pInvoice->AddDeduction(pDeductionItem);
	}

	std::vector<SellDocServerObject*> docSOs;
	
	try
	{
		pInsertedInvoice = m_DAO.InsertSellInvoice(transaction,m_pSeason->GetId(),pInvoice);
	
		BOOST_FOREACH(uint32_t docId,docIds)
		{
			m_DAO.UpdateSellDocWithInvoice(transaction,docId,pInsertedInvoice->GetId());
			docSOs.push_back(AcquireSellDoc(docId));
		}

	}
	catch(...)
	{
		transaction->Rollback();		
		if(pInsertedInvoice)
			delete pInsertedInvoice;

		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_SellInvoices.add(new SellInvoiceServerObject(pInsertedInvoice));
	observerTransaction->OnSellInvoiceInserted(pInsertedInvoice);

	uint32_t invoiceId = pInsertedInvoice->GetId();
	BOOST_FOREACH(SellDocServerObject* pDocSO,docSOs)
	{
		SellDoc* pSellDoc = pDocSO->Object();
		pSellDoc->SetInvoiceId(invoiceId);
		m_SellInvoiceRelations.AddRelationship(invoiceId,DocKey(DocType_Sell,pSellDoc->GetId()));
		observerTransaction->OnSellDocUpdated(pSellDoc);
		std::vector<CowServerObject*> cowSOs = GetCowSOsForDoc(pSellDoc);
		BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
		{
			observerTransaction->OnCowUpdated(pCowSO->Object());
		}
	}

	transaction->Commit();
	observerTransaction->Commit();

	m_pChangePublisher->PublishSellInvoiceInsert(pInsertedInvoice);
}

void SeasonSession::UpdateSellDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState(this);
	
	SellInvoiceServerObject* pOldInvoiceSO = AcquireSellInvoice(snapshot.GetInvoiceId());
	SellInvoice* pOldInvoice = pOldInvoiceSO->Object();
		
	SellInvoice* pReferenceInvoice = new SellInvoice(pOldInvoice->GetId(),InvoiceScope_Docs);
	std::auto_ptr<SellInvoice> _guard(pReferenceInvoice);

	pReferenceInvoice->SetFrom(snapshot.GetDetails());
	pReferenceInvoice->SetInvoiceNo(pOldInvoice->GetInvoiceNo());
	pReferenceInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	pReferenceInvoice->SetInvoiceType(snapshot.GetInvoiceType());
			
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pReferenceInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetFrom(entryDetails);		
	}

	BOOST_FOREACH(const IInvoiceDeduction* pDeductionItem, snapshot.Deductions())
	{
		pReferenceInvoice->AddDeduction(pDeductionItem);
	}

	std::set<uint32_t> oldInvoiceDocIds = GetExitDocIdsForInvoice(pOldInvoice);
	std::set<uint32_t> refInvoiceDocIds = GetExitDocIdsForInvoice(pReferenceInvoice);

	std::vector<SellDocServerObject*> oldInvoiceDocSOs = AcquireSellDocSOs(IdRange(oldInvoiceDocIds));
	std::vector<SellDocServerObject*> newInvoiceDocSOs = AcquireSellDocSOs(IdRange(refInvoiceDocIds));
	
	Diffrence<SellDocServerObject*> docDiffrence(oldInvoiceDocSOs,newInvoiceDocSOs);

	Diffrence<SellDocServerObject*>::Range addedDocs = docDiffrence.Added();
	Diffrence<SellDocServerObject*>::Range removedDocs = docDiffrence.Removed();	
	Diffrence<SellDocServerObject*>::Range commonDocs = docDiffrence.Common();	

	Transaction transaction = m_DAO.OpenTransaction();
	try
	{
		BOOST_FOREACH(SellDoc* pSellDoc,(addedDocs | boost::adaptors::transformed(ServerObjectPeeler<SellDocServerObject>())))
		{
			m_DAO.UpdateSellDocWithInvoice(transaction,pSellDoc->GetId(),pOldInvoice->GetId());			
		}

		BOOST_FOREACH(SellDoc* pSellDoc,(removedDocs | boost::adaptors::transformed(ServerObjectPeeler<SellDocServerObject>())))
		{
			m_DAO.UpdateSellDocWithInvoice(transaction,pSellDoc->GetId(),NULL_ID);			
		}

		m_DAO.UpdateSellInvoice(transaction,pReferenceInvoice);

	}
	catch(...)
	{
		transaction->Rollback();
		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	pOldInvoice->SetFrom(snapshot.GetDetails());
	pOldInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	
	InvoiceChangeDetector changeDetector(pOldInvoice,pReferenceInvoice);
	InvoiceChanges changes = changeDetector.Detect();

	BOOST_FOREACH(InvoiceChange* change,changes)
	{
		switch(change->Event())
		{
		case ChangeEvent_Added:
			pOldInvoice->AddEntry(new CowInvoiceEntry(change->New()));
			break;

		case ChangeEvent_Deleted:
			pOldInvoice->RemoveEntry(change->Old()->GetCowId());
			break;

		case ChangeEvent_Updated:
			pOldInvoice->GetEntry(change->Old()->GetCowId())->CopyFrom(change->New());
			break;
		}
	}

	pOldInvoice->RemoveAllDeductions();

	PtrEnumerator<IInvoiceDeduction> deductionEnum = pReferenceInvoice->EnumDeductions();
	while (deductionEnum.hasNext())
	{
		pOldInvoice->AddDeduction(*deductionEnum);
		deductionEnum.advance();
	}

	observerTransaction->OnSellInvoiceUpdated(pOldInvoice);

	uint32_t invoiceId = pOldInvoice->GetId();
	BOOST_FOREACH(SellDoc* pSellDoc,(addedDocs | boost::adaptors::transformed(ServerObjectPeeler<SellDocServerObject>())))
	{
		pSellDoc->SetInvoiceId(invoiceId);
		observerTransaction->OnSellDocUpdated(pSellDoc);
		m_SellInvoiceRelations.AddRelationship(invoiceId,DocKey(DocType_Sell,pSellDoc->GetId()));		
	}

	BOOST_FOREACH(SellDoc* pSellDoc,(removedDocs | boost::adaptors::transformed(ServerObjectPeeler<SellDocServerObject>())))
	{
		pSellDoc->SetInvoiceId(NULL_ID);
		observerTransaction->OnSellDocUpdated(pSellDoc);
		m_BuyInvoiceRelations.RemoveRelationship(invoiceId,DocKey(DocType_Sell,pSellDoc->GetId()));		
	}

	std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pOldInvoice);

	BOOST_FOREACH(Cow* pCow,(cowSOs | boost::adaptors::transformed(ServerObjectPeeler<CowServerObject>())))
	{
		observerTransaction->OnCowUpdated(pCow);
	}
		
	transaction->Commit();
	observerTransaction->Commit();

	m_pChangePublisher->PublishSellInvoiceUpdate(pOldInvoice);
}

void SeasonSession::InsertSellCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState state(this);

	Transaction transaction = m_DAO.OpenTransaction();
	
	SellInvoice* pInvoice = new SellInvoice(NULL_ID,InvoiceScope_Cows);
	std::auto_ptr<SellInvoice> newInvoiceDelete(pInvoice);

	uint32_t invoiceNo = ObtainNextSellInvoiceNo(transaction);

	pInvoice->SetInvoiceNo(invoiceNo);
	pInvoice->SetFrom(snapshot.GetDetails());
	pInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	pInvoice->SetInvoiceType(snapshot.GetInvoiceType());
	
	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();

	uint32_t docId = NULL_ID;
	
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetClassId(entryDetails.GetClassId());
		pEntry->SetPrice(entryDetails.GetPrice());
		pEntry->SetWeight(entryDetails.GetWeight());

		docId = pItem->GetDocId();
	}	

	std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pInvoice);
	SellInvoice* pInsertedInvoice = NULL;
	try
	{
		pInsertedInvoice = m_DAO.InsertSellInvoice(transaction,m_pSeason->GetId(),pInvoice);
	
		BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
		{
			m_DAO.UpdateCowWithSellInvoice(transaction,pCowSO->Object()->GetId(),pInsertedInvoice->GetId());			
		}

	}
	catch(...)
	{
		transaction->Rollback();		
		if(pInsertedInvoice)
			delete pInsertedInvoice;

		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	m_SellInvoices.add(new SellInvoiceServerObject(pInsertedInvoice));
	observerTransaction->OnSellInvoiceInserted(pInsertedInvoice);

	uint32_t invoiceId = pInsertedInvoice->GetId();
	BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
	{
		Cow* pCow = pCowSO->Object();
		pCow->SetSellInvoiceId(invoiceId);
		observerTransaction->OnCowUpdated(pCow);
	}

	m_SellInvoiceRelations.AddRelationship(invoiceId,DocKey(DocType_Out,docId));

	transaction->Commit();
	observerTransaction->Commit();

	m_pChangePublisher->PublishSellInvoiceInsert(pInsertedInvoice);
}

void SeasonSession::UpdateSellCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot)
{
	ValidateInvoiceDetails(snapshot.GetDetails());

	ScopedUpdateState(this);
	
	SellInvoiceServerObject* pOldInvoiceSO = AcquireSellInvoice(snapshot.GetInvoiceId());
	SellInvoice* pOldInvoice = pOldInvoiceSO->Object();
		
	SellInvoice* pReferenceInvoice = new SellInvoice(pOldInvoice->GetId(),InvoiceScope_Docs);
	std::auto_ptr<SellInvoice> _guard(pReferenceInvoice);

	pReferenceInvoice->SetFrom(snapshot.GetDetails());
	pReferenceInvoice->SetInvoiceNo(pOldInvoice->GetInvoiceNo());
	pReferenceInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());
	pReferenceInvoice->SetInvoiceType(snapshot.GetInvoiceType());

	InvoiceSnapshotItemsRange itemsRange = snapshot.Items();
	BOOST_FOREACH(const InvoiceCowItem* pItem,itemsRange)
	{
		CowInvoiceEntry* pEntry = pReferenceInvoice->AddEntry(pItem->GetCowId());
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		pEntry->SetFrom(entryDetails);		
	}

	InvoiceChangeDetector changeDetector(pOldInvoice,pReferenceInvoice);
	InvoiceChanges changes = changeDetector.Detect();
	uint32_t invoiceId = pOldInvoice->GetId();

	Transaction transaction = m_DAO.OpenTransaction();
	try
	{		
		BOOST_FOREACH(InvoiceChange* change,changes)
		{
			switch(change->Event())
			{
			case ChangeEvent_Added:
				m_DAO.UpdateCowWithSellInvoice(transaction,change->New()->GetCowId(),invoiceId);
				break;
			case ChangeEvent_Deleted:
				m_DAO.UpdateCowWithSellInvoice(transaction,change->Old()->GetCowId(),NULL_ID);								
				break;
			}
		}
		m_DAO.UpdateSellInvoice(transaction,pReferenceInvoice);
	}
	catch(...)
	{
		transaction->Rollback();
		boost::rethrow_exception(boost::current_exception());
	}

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();

	pOldInvoice->SetFrom(snapshot.GetDetails());
	pOldInvoice->SetInvoiceHent(snapshot.GetInvoiceHent());

	CowServerObject* pCowSO;
	Cow* pCow;
	
	Streak* pInvoiceUpdate = observerTransaction->StartStreak();
	Streak* pCowsUpdate = observerTransaction->StartStreak();

	BOOST_FOREACH(InvoiceChange* change,changes)
	{
		pCowSO = AcquireCow(change->GetCowId());
		pCow = pCowSO->Object();

		switch(change->Event())
		{
		case ChangeEvent_Added:
			pOldInvoice->AddEntry(new CowInvoiceEntry(change->New()));			
			pCow->SetSellInvoiceId(invoiceId);			
			break;

		case ChangeEvent_Deleted:			
			pOldInvoice->RemoveEntry(change->Old()->GetCowId());			
			pCow->SetSellInvoiceId(NULL_ID);			
			break;

		case ChangeEvent_Updated:
			pOldInvoice->GetEntry(change->Old()->GetCowId())->CopyFrom(change->New());			
			break;
		}
		pCowsUpdate->OnCowUpdated(pCow);
	}

	pOldInvoice->RemoveAllDeductions();

	PtrEnumerator<IInvoiceDeduction> deductionEnum = pReferenceInvoice->EnumDeductions();
	while (deductionEnum.hasNext())
	{
		pOldInvoice->AddDeduction(*deductionEnum);
		deductionEnum.advance();
	}

	pInvoiceUpdate->OnSellInvoiceUpdated(pOldInvoice);	 	
	transaction->Commit();
	observerTransaction->Commit();

	m_pChangePublisher->PublishSellInvoiceUpdate(pOldInvoice);
}

void SeasonSession::DeleteSellInvoice(ClientSeasonSession* pClient,uint32_t invoiceId)
{
	ScopedUpdateState(this);

	SellInvoiceServerObject* pSellInvoiceSO = AcquireSellInvoice(invoiceId);

	if(!pSellInvoiceSO->OwnsLock(pClient))
		ThrowInvoiceLockNotObtained(invoiceId);

	SellInvoice* pSellInvoice =  pSellInvoiceSO->Object();
	
	ThrowIfInvoiceIsPaid(pSellInvoice);

	ObserverTransactionPtr observerTransaction = m_pSpectator->OpenTransaction();
	Transaction transaction = m_DAO.OpenTransaction();

	switch(pSellInvoice->GetInvoiceScope())
	{
		case InvoiceScope_Docs:
		{
			std::vector<DocKey> keys = m_SellInvoiceRelations.GetInvoiceRelationsOf(invoiceId,DocType_Sell);
			std::vector<SellDocServerObject*> docSOs;
			BOOST_FOREACH(DocKey key,keys)
			{				
				docSOs.push_back(FindSellDoc(key.GetDocNo()));
				m_DAO.UpdateSellDocWithInvoice(transaction,key.GetDocNo(),NULL_ID);				
			}			
			m_DAO.DeleteSellInvoice(transaction,invoiceId);			

			BOOST_FOREACH(SellDocServerObject* pDocSO,docSOs)
			{				
				SellDoc* pDoc = pDocSO->Object();
				pDoc->SetInvoiceId(NULL_ID);
				observerTransaction->OnSellDocUpdated(pDoc);		

				std::vector<CowServerObject*> cowSOs = GetCowSOsForDoc(pDoc);
				BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
				{
					Cow* pCow = pCowSO->Object();
					observerTransaction->OnCowUpdated(pCow);		
					Unlock(pClient,pCowSO);
					observerTransaction->OnCowUnlock(pCow->GetId());		
				}
				Unlock(pClient,pDocSO);
				observerTransaction->OnSellDocUnlock(pDoc->GetId());						
			}
			m_SellInvoiceRelations.RemoveRelations(invoiceId,keys);

			break;
		}

		case InvoiceScope_Cows:
		{			
			std::vector<CowServerObject*> cowSOs = GetCowSOsForInvoice(pSellInvoice);
			DocKey key = m_SellInvoiceRelations.GetInvoiceRelationship(invoiceId);
			OutDocServerObject* pDocSO = AcquireOutDoc(key.GetDocNo());

			BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
			{					
				m_DAO.UpdateCowWithSellInvoice(transaction,pCowSO->Object()->GetId(),NULL_ID);				
			}			
			m_DAO.DeleteSellInvoice(transaction,invoiceId);			

			BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
			{	
				Cow* pCow = pCowSO->Object();
				pCow->SetSellInvoiceId(NULL_ID);
				observerTransaction->OnCowUpdated(pCowSO->Object());						
			}

			cowSOs = GetCowSOsForDoc(pDocSO->Object());
			BOOST_FOREACH(CowServerObject* pCowSO,cowSOs)
			{
				Unlock(pClient,pCowSO);
				observerTransaction->OnCowUnlock(pCowSO->Object()->GetId());			
			}
					
			Unlock(pClient,pDocSO);
			observerTransaction->OnOutDocUnlock(pDocSO->Object()->GetId());			
			m_SellInvoiceRelations.RemoveRelationship(invoiceId,key);
			break;
		}
		default:
			ThrowFatalError(__WFILE__,__LINE__,L"invalid scope");
	}

	observerTransaction->OnSellInvoiceDelete(invoiceId);
	Unlock(pClient,pSellInvoiceSO);
	m_SellInvoices.remove(invoiceId);
	delete pSellInvoiceSO;

	observerTransaction->Commit();
	transaction->Commit();

	m_pChangePublisher->PublishSellInvoiceDelete(invoiceId);
}

void SeasonSession::ThrowFatalError(const std::wstring& file,int line, const std::wstring& message)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::Format(L"%s:%d - %s", std::wstring(file.begin(), file.end()))));
}

void SeasonSession::ThrowInvoiceLockNotObtained(uint32_t invoiceId)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"invoice update id=%d without acquiring the lock", invoiceId)));
}

IClass* SeasonSession::FindClass(const std::wstring& classCd)
{
	return m_pFarmSession->FindClass(classCd);
}

IStock* SeasonSession::FindStock(const std::wstring& stockCd)
{
	return m_pFarmSession->FindStock(stockCd);
}

IAgent* SeasonSession::FindAgent(const std::wstring&  agentCd)
{
	return m_pFarmSession->FindAgent(agentCd);
}

uint32_t SeasonSession::ObtainNextBuyInvoiceNo(Transaction transaction)
{
	const wchar_t* counterName = L"BUY_INVOICE_NO";
	m_DAO.EnsureCounter(transaction, counterName, m_pSeason->GetId());
	return m_DAO.NextCounterValue(transaction, counterName, m_pSeason->GetId());
}

uint32_t SeasonSession::ObtainNextSellInvoiceNo(Transaction transaction)
{
	const wchar_t* counterName = L"SELL_INVOICE_NO";
	m_DAO.EnsureCounter(transaction, counterName, m_pSeason->GetId());
	return m_DAO.NextCounterValue(transaction, counterName, m_pSeason->GetId());
}

void SeasonSession::InsertPurchase(IPurchase* purchase, const DateTime& docDt)
{
	m_logger->i(L"inserting purchase");

	BkLoggerAdapter loggerAdapter(m_logger);
	InsertPurchaseOperation operation(purchase,this, &loggerAdapter);
	operation.SetDocDate(docDt);
	operation.execute();	
}


void SeasonSession::ThrowIfThereAreInvoices(IBuyDoc* pDoc)
{
	if (pDoc->GetInvoiceId() != NULL_ID)
	{
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"buydoc_has_invoice").str()));
	}
}

void SeasonSession::ThrowIfThereAreInvoices(ISellDoc* pDoc)
{
	if (pDoc->GetInvoiceId() != NULL_ID)
	{
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"selldoc_has_invoice").str()));
	}
}

void SeasonSession::ThrowIfThereAreInvoices(IInDoc* pDoc)
{
	std::vector<CowServerObject*> cowSOs = GetCowSOsForDoc(pDoc);
	BOOST_FOREACH(CowServerObject* cowSO, cowSOs)
	{
		Cow* cow = cowSO->Object();
		if (cow->GetBuyInvoiceId() != NULL_ID)
		{
			BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"indoc_cow_has_invoice")(L"cow",cow->GetCowNo().ToString()).str()));
		}
	}
}

void SeasonSession::ThrowIfThereAreInvoices(IOutDoc* pDoc)
{
	std::vector<CowServerObject*> cowSOs = GetCowSOsForDoc(pDoc);
	BOOST_FOREACH(CowServerObject* cowSO, cowSOs)
	{
		Cow* cow = cowSO->Object();
		if (cow->GetSellInvoiceId() != NULL_ID)
		{
			BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"outdoc_cow_has_invoice")(L"cow", cow->GetCowNo().ToString()).str()));
		}
	}
}