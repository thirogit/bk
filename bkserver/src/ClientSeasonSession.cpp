#include "stdafx.h"
#include "ClientSeasonSession.h"
#include "SeasonSession.h"
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "object/HentServerObject.h"
#include "SessionException.h"
#include "transaction/NewInDocTransaction.h"
#include "transaction/EditInDocTransaction.h"
#include "transaction/NewMoveDocTransaction.h"
#include "transaction/EditMoveDocTransaction.h"
#include "transaction/NewSellDocTransaction.h"
#include "transaction/NewBuyDocTransaction.h"
#include "transaction/EditBuyDocTransaction.h"
#include "transaction/OutDocTransaction.h"
#include "transaction/EditSellDocTransaction.h"
#include "transaction/BuyInvoiceDocsTransaction.h"
#include "transaction/BuyInvoiceCowsTransaction.h"
#include <string/TextUtils.h>
#include <boost/range/adaptor/map.hpp>
#include <boost/foreach.hpp>
#include "transaction/SellInvoiceDocsTransaction.h"
#include "transaction/SellInvoiceCowsTransaction.h"
#include "repo/RepoClientFactory.h"
#include "repo/RepoException.h"
#include "transaction/purchase/DownloadPurchaseTransaction.h"


ClientSeasonSession::SeasonInterface::SeasonInterface(ClientSeasonSession* parent) : m_parent(parent)
{
}

void ClientSeasonSession::SeasonInterface::Do(std::function<void (SeasonSessionFacade*)>& action)
{
	m_parent->m_pInterface->Do(m_parent,action);
}

void ClientSeasonSession::SeasonInterface::DoUser(std::function<void(UserSessionFacade*)>& action)
{
	m_parent->m_pClientSession->UserDoAny(action);
}

//-------------------------------------------------------------------------------------------------
ClientSeasonSession::SeasonLogInterface::SeasonLogInterface(ClientSeasonSession* parent) : m_parent(parent)
{
}

void ClientSeasonSession::SeasonLogInterface::Log(LogLevel level, const std::wstring& msg)
{
	switch (level)
	{
	default:
	case LogLevel_Debug:
		m_parent->m_pClientSession->d(FormatSeasonMessage(msg));
		break;
	case LogLevel_Info:
		m_parent->m_pClientSession->i(FormatSeasonMessage(msg));
		break;
	case LogLevel_Error:
		m_parent->m_pClientSession->e(FormatSeasonMessage(msg));
		break;
	}
}

std::wstring ClientSeasonSession::SeasonLogInterface::FormatSeasonMessage(const std::wstring& msg) const
{
	return TextUtils::Format(L"%d: %s", m_parent->m_SessionToken, msg);
}
//-------------------------------------------------------------------------------------------------
ClientSeasonSession::ClientSeasonSession(ClientSession* pClientSession,ClientSeasonSessionInterface* pInterface,uint32_t sessionToken) : m_TransactionIdSequence(1)
{
	m_SessionToken = sessionToken;
	m_pInterface = pInterface;
	m_pClientSession = pClientSession;
	m_pSeasonInterface = new SeasonInterface(this);
	m_pLog = new SeasonLogInterface(this);
	m_pClient = NULL;
	
}

ClientSeasonSession::~ClientSeasonSession()
{
	delete m_pSeasonInterface;
	delete m_pLog;
}

uint32_t ClientSeasonSession::GetSessionToken() const
{
	return m_SessionToken;
}

uint32_t ClientSeasonSession::GetSeasonId() const
{
	return m_pInterface->GetSeason()->GetId();
}

uint16_t ClientSeasonSession::GetObserverPort() const
{
	return m_pInterface->GetObserverPort();
}

ClientSession* ClientSeasonSession::GetClientSession() const
{
	return m_pClientSession;
}

void ClientSeasonSession::ThrowIfNotInFetchState()
{
//	if(m_pSeasonSession->GetSessionState() != Fetch_In_Progress)
//		BOOST_THROW_EXCEPTION(SessionException("not_in_fetch"));
}

FetchHandle ClientSeasonSession::BeginFetch()
{	
	m_FetchHandle = m_pInterface->OpenFetchHandle();
	return m_FetchHandle;
}

FetchHandle ClientSeasonSession::GetFetchHandle()
{
	return m_FetchHandle;
}

void ClientSeasonSession::EndFetch()
{
	m_FetchHandle.reset();
}

void ClientSeasonSession::LockHent(uint32_t hentId)
{
	SeasonDoAny([hentId](SeasonSessionFacade* facade)
												{
													facade->LockHent(hentId);
												});	
}

void ClientSeasonSession::UnlockHent(uint32_t hentId)
{
	SeasonDoAny([hentId](SeasonSessionFacade* facade)
												{
													facade->UnlockHent(hentId);
												});
}

uint32_t ClientSeasonSession::InsertHent(IHent* pHent)
{
	return SeasonEval<uint32_t>([pHent](SeasonSessionFacade* facade) -> uint32_t
												{
													return facade->InsertHent(pHent);
												});
}

void ClientSeasonSession::UpdateHent(IHent* pHent)
{
	SeasonDoAny([pHent](SeasonSessionFacade* facade)
												{
													facade->UpdateHent(pHent);
												});
}

void ClientSeasonSession::DeleteHent(uint32_t hentId)
{
	SeasonDoAny([hentId](SeasonSessionFacade* facade)
												{
													facade->DeleteHent(hentId);
												});
}

void ClientSeasonSession::LockCow(uint32_t cowId)
{
	SeasonDoAny([cowId](SeasonSessionFacade* facade)
												{
													facade->LockCow(cowId);
												});
}

void ClientSeasonSession::UnlockCow(uint32_t cowId)
{
	SeasonDoAny([cowId](SeasonSessionFacade* facade)
												{
													facade->UnlockCow(cowId);
												});
}

void ClientSeasonSession::UpdateCow(uint32_t cowId,const CowDetails& details)
{
	SeasonDoFacade(&SeasonSessionFacade::UpdateCow,cowId,details);
}

uint32_t ClientSeasonSession::ObtainNextTransactionId()
{
	return m_TransactionIdSequence.fetch_add(1);
}

void ClientSeasonSession::ThrowTransactionNotFound()
{
	BOOST_THROW_EXCEPTION(SessionException(L"transaction_not_found"));
}



IClientTransaction* ClientSeasonSession::OpenNewInDocTransaction(uint32_t herdId)
{
	IHerd* pHerd = AcquireHerd(herdId);
	NewInDocTransaction* pTransaction = new NewInDocTransaction(m_pSeasonInterface, m_pLog,ObtainNextTransactionId(), herdId);

	m_Transactions[pTransaction->GetId()] = pTransaction;

	return pTransaction;
}

IHerd* ClientSeasonSession::AcquireHerd(uint32_t herdId)
{
	IHerd* pHerd = m_pInterface->FindHerd(herdId);
	if(!pHerd)
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"invalid_herd, id=%d",herdId)));
	return pHerd;
}

IClientTransaction* ClientSeasonSession::OpenEditInDocTransaction(uint32_t docId)
{	
	bool locked =   SeasonEval<bool>([docId](SeasonSessionFacade* facade) -> bool
					{
						return facade->IsInDocLocked(docId);
					});

	if(!locked)
		ThrowLockOnDocNotAcquiredForTransaction(DocKey(DocType_In,docId));

	EditInDocTransaction* pTransaction = new EditInDocTransaction(m_pSeasonInterface, m_pLog,ObtainNextTransactionId());
	pTransaction->Edit(docId);
	m_Transactions[pTransaction->GetId()] = pTransaction;
	return pTransaction;	
}

IClientTransaction* ClientSeasonSession::FindTransaction(uint32_t transactionId)
{
	auto transactionIt = m_Transactions.find(transactionId);
	if(transactionIt != m_Transactions.end())
		return transactionIt->second;

	return NULL;
}

IClientTransaction* ClientSeasonSession::AcquireTransaction(uint32_t transactionId)
{
	IClientTransaction* pTransaction = FindTransaction(transactionId);
	if(!pTransaction)
		ThrowTransactionNotFound();

	return pTransaction;
}

void  ClientSeasonSession::DisposeTransaction(uint32_t transactionId)
{
	auto transactionIt = m_Transactions.find(transactionId);
	if(transactionIt != m_Transactions.end())
	{
		delete transactionIt->second;
		m_Transactions.erase(transactionIt);
		return;
	}
	ThrowTransactionNotFound();
}

IClientTransaction* ClientSeasonSession::OpenNewMoveDocTransaction(uint32_t srcHerdId,uint32_t dstHerdId)
{
	AcquireHerd(srcHerdId);
	AcquireHerd(dstHerdId);
	uint32_t transactionId = ObtainNextTransactionId();
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Opening new sell doc transaction, srcHerdId = %1%, dstHerdId = %2%, id = %3%", srcHerdId, dstHerdId, transactionId));

	NewMoveDocTransaction* pTransaction = new NewMoveDocTransaction(m_pSeasonInterface, m_pLog, transactionId, srcHerdId, dstHerdId);
	m_Transactions[pTransaction->GetId()] = pTransaction;

	return pTransaction;
}

IClientTransaction* ClientSeasonSession::OpenEditMoveDocTransaction(uint32_t docId)
{
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"About to open edit move doc transaction, docId = %1%, checking for lock", docId));
	bool locked =   SeasonEval<bool>([docId](SeasonSessionFacade* facade) -> bool
					{
						return facade->IsMoveDocLocked(docId);
					});

	if (!locked)
	{
		m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Lock was not aquired for docId = %1% before opening transaction, throwing", docId));
		ThrowLockOnDocNotAcquiredForTransaction(DocKey(DocType_Move, docId));
	}

	uint32_t transactionId = ObtainNextTransactionId();
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Opening edit move doc transaction, docId = %1%, id = %2%", docId, transactionId));

	EditMoveDocTransaction* pTransaction = new EditMoveDocTransaction(m_pSeasonInterface, m_pLog, transactionId);
	pTransaction->Edit(docId);

	m_Transactions[pTransaction->GetId()] = pTransaction;
	return pTransaction;	
}

IClientTransaction* ClientSeasonSession::OpenNewSellDocTransaction(uint32_t herdId)
{
	AcquireHerd(herdId);
	
	uint32_t transactionId = ObtainNextTransactionId();
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Opening new sell doc transaction, herdId = %1%, id = %2%", herdId, transactionId));
	
	NewSellDocTransaction* pTransaction = new NewSellDocTransaction(m_pSeasonInterface, m_pLog, transactionId, herdId);
	m_Transactions[pTransaction->GetId()] = pTransaction;

	return pTransaction;
}

IClientTransaction* ClientSeasonSession::OpenEditSellDocTransaction(uint32_t docId)
{
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"About to open edit sell doc transaction, docId = %1%, checking for lock", docId));
	bool locked =   SeasonEval<bool>([docId](SeasonSessionFacade* facade) -> bool
					{
						return facade->IsSellDocLocked(docId);
					});

	if (!locked)
	{
		m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Lock was not aquired for docId = %1% before opening transaction, throwing", docId));
		ThrowLockOnDocNotAcquiredForTransaction(DocKey(DocType_Sell, docId));
	}

	uint32_t transactionId = ObtainNextTransactionId();
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Opening edit sell doc transaction, docId = %1%, id = %2%", docId, transactionId));
	
	EditSellDocTransaction* pTransaction = new EditSellDocTransaction(m_pSeasonInterface, m_pLog, transactionId);
	pTransaction->Edit(docId);

	m_Transactions[pTransaction->GetId()] = pTransaction;
	return pTransaction;
}

IClientTransaction* ClientSeasonSession::OpenNewBuyDocTransaction(uint32_t herdId)
{
	AcquireHerd(herdId);

	uint32_t transactionId = ObtainNextTransactionId();
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Opening new buy doc transaction, herdId = %1%, id = %2%", herdId, transactionId));

	NewBuyDocTransaction* pTransaction = new NewBuyDocTransaction(m_pSeasonInterface, m_pLog, transactionId, herdId);
	m_Transactions[pTransaction->GetId()] = pTransaction;

	return pTransaction;
}

IClientTransaction* ClientSeasonSession::OpenEditBuyDocTransaction(uint32_t docId)
{
	EditBuyDocTransaction* pTransaction = new EditBuyDocTransaction(m_pSeasonInterface, m_pLog, ObtainNextTransactionId());
	pTransaction->Edit(docId);
	m_Transactions[pTransaction->GetId()] = pTransaction;

	return pTransaction;
}

IClientTransaction* ClientSeasonSession::OpenNewOutDocTransaction(uint32_t herdId)
{
	AcquireHerd(herdId);
	
	uint32_t transactionId = ObtainNextTransactionId();
	m_pLog->Log(LogLevel_Info, TextUtils::Format(L"Opening new out doc transaction, herdId = %1%, id = %2%", herdId, transactionId));

	OutDocTransaction* pTransaction = new OutDocTransaction(m_pSeasonInterface, m_pLog, transactionId);
	pTransaction->New(herdId);
	m_Transactions[pTransaction->GetId()] = pTransaction;
	return pTransaction;
}

IClientTransaction* ClientSeasonSession::OpenEditOutDocTransaction(uint32_t docId)
{
	OutDocTransaction* pTransaction = new OutDocTransaction(m_pSeasonInterface, m_pLog, ObtainNextTransactionId());
	pTransaction->Edit(docId);
	m_Transactions[pTransaction->GetId()] = pTransaction;
	return pTransaction;
}

void ClientSeasonSession::ThrowLockOnDocNotAcquiredForTransaction(const DocKey& docKey)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"lock not acquired on doc %c%05d for transaction",docKey.GetDocType(),docKey.GetDocNo())));
}

void ClientSeasonSession::ThrowLockOnInvoiceNotAcquiredForTransaction(const InvoiceKey& invoiceKey)
{
	BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"lock not acquired on invoice %c%05d for transaction",invoiceKey.GetDirection(),invoiceKey.GetInvoiceId())));
}

IClientTransaction* ClientSeasonSession::OpenNewBuyInvoiceTransaction(InvoiceScope scope)
{
	IClientTransaction* transaction = NULL;

	switch(scope)
	{
		case InvoiceScope_Docs:
		{
			BuyInvoiceDocsTransaction* pTransaction = new BuyInvoiceDocsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->New();
			transaction = pTransaction; 
			break;			
		}

		case InvoiceScope_Cows:
		{
			BuyInvoiceCowsTransaction* pTransaction = new BuyInvoiceCowsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->New();
			transaction = pTransaction; 
			break;
		}	
		default:
			ThrowUnsupportedInvoiceScope();
	}

	m_Transactions[transaction->GetId()] = transaction;
	return transaction;		
}

void ClientSeasonSession::ThrowUnsupportedInvoiceScope()
{
	BOOST_THROW_EXCEPTION(SessionException(L"unsupported invoice scope"));
}

IClientTransaction* ClientSeasonSession::OpenNewSellInvoiceTransaction(InvoiceScope scope)
{
	IClientTransaction* transaction = NULL;
	switch(scope)
	{
		case InvoiceScope_Docs:
		{
			SellInvoiceDocsTransaction* pTransaction = new SellInvoiceDocsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->New();
			transaction = pTransaction;	
			break;
		}

		case InvoiceScope_Cows:
		{
			SellInvoiceCowsTransaction* pTransaction = new SellInvoiceCowsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->New();			
			transaction = pTransaction;		
			break;
		}
		default:
			ThrowUnsupportedInvoiceScope();
	}

	m_Transactions[transaction->GetId()] = transaction;
	return transaction;
}

IClientTransaction* ClientSeasonSession::OpenEditBuyInvoiceTransaction(uint32_t invoiceId)
{
	InvoiceScope scope = SeasonEval<InvoiceScope>([this,invoiceId](SeasonSessionFacade* facade)
												{
													if(!facade->IsBuyInvoiceLocked(invoiceId))
														ThrowLockOnInvoiceNotAcquiredForTransaction(InvoiceKey(InvoiceDirection_Buy,invoiceId));

													IBuyInvoice* pInvoice = facade->FindBuyInvoice(invoiceId);
													return pInvoice->GetInvoiceScope();
												});

	switch(scope)
	{
	case InvoiceScope_Docs:
		{
			BuyInvoiceDocsTransaction* pTransaction = new BuyInvoiceDocsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->Edit(invoiceId);
			m_Transactions[pTransaction->GetId()] = pTransaction;
			return pTransaction;
		}
	case InvoiceScope_Cows:
		{
			BuyInvoiceCowsTransaction* pTransaction = new BuyInvoiceCowsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->Edit(invoiceId);
			m_Transactions[pTransaction->GetId()] = pTransaction;
			return pTransaction;
		}
	}

	return NULL;
}

void ClientSeasonSession::DeleteBuyInvoice(uint32_t invoiceId)
{
	SeasonDoFacade(&SeasonSessionFacade::DeleteBuyInvoice,invoiceId);												
}
	
IClientTransaction* ClientSeasonSession::OpenEditSellInvoiceTransaction(uint32_t invoiceId)
{
	InvoiceScope scope = SeasonEval<InvoiceScope>([this,invoiceId](SeasonSessionFacade* facade)
											{
												if(!facade->IsSellInvoiceLocked(invoiceId))
													ThrowLockOnInvoiceNotAcquiredForTransaction(InvoiceKey(InvoiceDirection_Sell,invoiceId));

												ISellInvoice* pInvoice = facade->FindSellInvoice(invoiceId);
												return pInvoice->GetInvoiceScope();
											});

	switch(scope)
	{
	case InvoiceScope_Docs:
		{
			SellInvoiceDocsTransaction* pTransaction = new SellInvoiceDocsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->Edit(invoiceId);
			m_Transactions[pTransaction->GetId()] = pTransaction;
			return pTransaction;
		}
	case InvoiceScope_Cows:
		{
			SellInvoiceCowsTransaction* pTransaction = new SellInvoiceCowsTransaction(m_pSeasonInterface,ObtainNextTransactionId());
			pTransaction->Edit(invoiceId);
			m_Transactions[pTransaction->GetId()] = pTransaction;
			return pTransaction;
		}
	}

	return NULL;
}

void ClientSeasonSession::DeleteSellInvoice(uint32_t invoiceId)
{
	SeasonDoFacade(&SeasonSessionFacade::DeleteSellInvoice,invoiceId);												
}

void ClientSeasonSession::CommitTransaction(uint32_t transactionId)
{
	AcquireTransaction(transactionId)->Commit();
	DisposeTransaction(transactionId);
}

void ClientSeasonSession::AbortTransaction(uint32_t transactionId)
{
	AcquireTransaction(transactionId)->Abort();
	DisposeTransaction(transactionId);
}

boost::any ClientSeasonSession::ExecuteOperation(uint32_t transactionId,uint32_t operation,const OperationArguments& args)
{
	try
	{
		return AcquireTransaction(transactionId)->ExecuteOperation(operation, args);
	}
	catch (...)
	{
		boost::exception_ptr eptr = boost::current_exception();
		std::wstring errorMsg;

		wexception* e_wexception = boost::current_exception_cast<wexception>();
		if (e_wexception != NULL)
		{
			errorMsg = e_wexception->wwhat();
		}
		else
		{
			boost::bad_any_cast* e_bad_any_cast = boost::current_exception_cast<boost::bad_any_cast>();
			if (e_bad_any_cast != NULL)
			{
				errorMsg = L"bad any cast";
			}
			else
			{				
				errorMsg = L"unknown exception - " + TextUtils::FromUTF8( boost::diagnostic_information(eptr,false));
			}
		}

		m_pLog->Log(LogLevel_Error, TextUtils::Format(L"client session %d: got error '%s' while executing operation %d on transaction %d", m_SessionToken, errorMsg, operation, transactionId));

		
		
		boost::rethrow_exception(eptr);
	}

	return boost::any();
}

void ClientSeasonSession::LockInDocWithCows(uint32_t docId)
{
	SeasonDoFacade(&SeasonSessionFacade::LockInDocWithCows, docId);
}

void ClientSeasonSession::UnlockInDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->UnlockInDoc(docId);
												});
}

void ClientSeasonSession::DeleteInDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->DeleteInDoc(docId);
												});
}

void ClientSeasonSession::LockBuyDoc(uint32_t docId)
{
	SeasonDoFacade(&SeasonSessionFacade::LockBuyDoc,docId);												
}

void ClientSeasonSession::UnlockBuyDoc(uint32_t docId)
{
	SeasonDoFacade(&SeasonSessionFacade::UnlockBuyDoc,docId);												
}

void ClientSeasonSession::DeleteBuyDoc(uint32_t docId)
{
	SeasonDoFacade(&SeasonSessionFacade::DeleteBuyDoc,docId);												
}

void ClientSeasonSession::LockMoveDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->LockMoveDoc(docId);
												});
}
void ClientSeasonSession::UnlockMoveDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->UnlockMoveDoc(docId);
												});
}
void ClientSeasonSession::DeleteMoveDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->DeleteMoveDoc(docId);
												});
}

void ClientSeasonSession::LockOutDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->LockOutDoc(docId);
												});
}
void ClientSeasonSession::UnlockOutDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->UnlockOutDoc(docId);
												});
}
void ClientSeasonSession::DeleteOutDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->DeleteOutDoc(docId);
												});
}

void ClientSeasonSession::LockSellDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->LockSellDoc(docId);
												});
}
void ClientSeasonSession::UnlockSellDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->UnlockSellDoc(docId);
												});
}	
void ClientSeasonSession::DeleteSellDoc(uint32_t docId)
{
	SeasonDoAny([docId](SeasonSessionFacade* facade)
												{
													facade->DeleteSellDoc(docId);
												});
}

void ClientSeasonSession::LockBuyInvoice(uint32_t invoiceId)
{
	SeasonDoFacade(&SeasonSessionFacade::LockBuyInvoice,invoiceId);		
}

void ClientSeasonSession::UnlockBuyInvoice(uint32_t invoiceId)
{
	SeasonDoFacade(&SeasonSessionFacade::UnlockBuyInvoice,invoiceId);		
}

void ClientSeasonSession::LockSellInvoice(uint32_t invoiceId)
{
	SeasonDoFacade(&SeasonSessionFacade::LockSellInvoice,invoiceId);		
}

void ClientSeasonSession::UnlockSellInvoice(uint32_t invoiceId)
{
	SeasonDoFacade(&SeasonSessionFacade::UnlockSellInvoice,invoiceId);		
}

void ClientSeasonSession::Close()
{
	m_pLog->Log(LogLevel_Debug, TextUtils::Format(L"closing client season session %d", m_SessionToken));
	m_purchasesCursor.reset();


	BOOST_FOREACH(IClientTransaction* transaction,(m_Transactions | boost::adaptors::map_values))
	{
		try
		{
			m_pLog->Log(LogLevel_Debug,TextUtils::Format(L"%d session close: aborting transaction %d", m_SessionToken, transaction->GetId()));
			transaction->Abort();
		}
		catch(...)
		{
			m_pLog->Log(LogLevel_Error, TextUtils::Format(L"%d session close: got error while closing transaction %d", m_SessionToken, transaction->GetId()));
		}

		delete transaction;
	}	

	delete m_pClient;
	m_pClient = NULL;

	m_Transactions.clear();
	m_pInterface->Close(this);
}


void ClientSeasonSession::OpenPurchaseRepository()
{
	m_pLog->Log(LogLevel_Debug, L"opening repository");
	if (!m_pClient)
	{
		try
		{			
			m_pClient = m_pClientSession->GetRepoClientFactory()->CreateClient(m_pInterface->GetFarm()->GetFarmNo());
			//m_pClient = m_pClientSession->GetRepoClientFactory()->CreateClient(FarmNoFactory::FromString(L"PL067097631"));
		}
		catch (repo::RepoException& e)
		{
			m_pLog->Log(LogLevel_Debug, L"failed to open repository: " + e.message());
			BOOST_THROW_EXCEPTION( SessionException(e.wwhat()) );
		}		
	}
	else
	{
		m_pLog->Log(LogLevel_Debug, L"already open");
	}
}

int ClientSeasonSession::QueryPurchases(const DateTime& startDt, const DateTime& endDt)
{
	ThrowIfNoRepoSession();

	m_purchasesCursor.reset();

	m_pLog->Log(LogLevel_Debug, L"quering purchases between " + startDt.GetDayDate() + L" and " + endDt.GetDayDate());

	try
	{
		m_purchasesCursor = m_pClient->QueryPurchases(startDt, endDt);

		const repo::CursorFetchLimit& limit = m_purchasesCursor->GetLimits();

		int count = m_purchasesCursor->GetCount();
		int maxfetch = limit.GetMaxFetch();
		int fetchsteps = (count / maxfetch) + ((count%maxfetch > 0) ? 1 : 0);

		m_pLog->Log(LogLevel_Debug, TextUtils::Format(L"purchase count %d, max fetch %d, fetch steps %d", count, maxfetch, fetchsteps));

		if (count > 0)
		{
			return fetchsteps;
		}
		return 0;
	}
	catch (repo::RepoException& e)
	{
		m_pLog->Log(LogLevel_Debug, L"quering failed: " + e.message());
		BOOST_THROW_EXCEPTION(SessionException(e.wwhat()));
	}
}

PurchaseHeaders ClientSeasonSession::FetchPurchaseHeaders()
{
	ThrowIfNoRepoSession();
	if (!m_purchasesCursor)
		BOOST_THROW_EXCEPTION(SessionException(L"no_active_purchase_query"));

	const repo::CursorFetchLimit limits = m_purchasesCursor->GetLimits();
	try
	{
		int maxfetch = limits.GetMaxFetch();
		m_pLog->Log(LogLevel_Debug, TextUtils::Format(L"fetching max %d purchase headers", maxfetch));
		PurchaseHeaders result(std::move(m_purchasesCursor->FetchMany(maxfetch)));
		m_pLog->Log(LogLevel_Debug, TextUtils::Format(L"fetched %d purchase headers", result.Count()));
		if (result.Count() == 0)
		{
			m_purchasesCursor.reset();
		}

		return std::move(result);
	}
	catch (repo::RepoException& e)
	{
		m_pLog->Log(LogLevel_Debug, L"fetch unsuccessful: " + e.message());
		BOOST_THROW_EXCEPTION(SessionException(e.wwhat()));
	}

	return PurchaseHeaders();
}

boost::optional<Purchase> ClientSeasonSession::FetchPurchase(const std::wstring& purchaseId)
{
	ThrowIfNoRepoSession();
	try
	{
		m_pLog->Log(LogLevel_Debug, TextUtils::Format(L"fetching purchase %s", purchaseId));	
		return std::move(m_pClient->GetPurchase(purchaseId));
	}
	catch (repo::RepoException& e)
	{
		m_pLog->Log(LogLevel_Debug, L"fetch unsuccessful: " + e.message());
		BOOST_THROW_EXCEPTION(SessionException(e.wwhat()));
	}

	return boost::optional<Purchase>();
}


IClientTransaction* ClientSeasonSession::OpenDownloadPurchaseTransaction(const std::wstring& purchaseId)
{
	ThrowIfNoRepoSession();

	m_pLog->Log(LogLevel_Debug, TextUtils::Format(L"opening download transaction for purchase with id %d", purchaseId));
	DownloadPurchaseTransaction* transaction = 
		new DownloadPurchaseTransaction(m_pSeasonInterface, m_pLog, m_pClient,
										ObtainNextTransactionId());
	transaction->Open(purchaseId);
	m_Transactions[transaction->GetId()] = transaction;
	return transaction;
}

void ClientSeasonSession::ThrowIfNoRepoSession()
{
	
}

void ClientSeasonSession::ClosePurchaseRepository()
{
	m_pLog->Log(LogLevel_Debug, L"closing repository.");
	if (m_pClient)
	{	
		delete m_pClient;
		m_pClient = NULL;
	}
}