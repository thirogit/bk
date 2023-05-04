#include "stdafx.h"
#include "BKService.h"
#include <RCF/RCF.hpp>
#include "SessionException.h"
#include "Log.h"
#include <boost/foreach.hpp>
#include "ClientSession.h"
#include <string/Utf8.h>

ClientSessionHolder::ClientSessionHolder() : m_pSession(NULL)
{
}

void ClientSessionHolder::SetClientSession(ClientSession* pSession)
{
	m_pSession = pSession;
}

ClientSessionHolder::~ClientSessionHolder()
{
	m_pSession->CloseAllSessions();	
	delete m_pSession;
}

ClientSession* ClientSessionHolder::GetClientSession() const
{
	return m_pSession;
}


BKService::BKService() : m_publisher(NULL)
{

}


ClientSession* BKService::AquireClientSession()
{
	RCF::RcfSession & session = RCF::getCurrentRcfSession();
	ClientSessionHolder * pSessionHolder = session.querySessionObject<ClientSessionHolder>();
	if(!pSessionHolder)
		BOOST_THROW_EXCEPTION(SessionException(L"not logged in"));

	return pSessionHolder->GetClientSession();
}

LoginResponseObj BKService::Login(LoginRequestObj pRequest)
{
	RCF::RcfSession & session = RCF::getCurrentRcfSession();
	ClientSessionHolder * pSessionHolder = session.querySessionObject<ClientSessionHolder>();

	if(pSessionHolder)
		BOOST_THROW_EXCEPTION(SessionException(L"already_logged_in"));
		
	Log::I(Utf8::FromUTF8(session.getClientAddress().string()),
		(boost::wformat(L"Received login request for user %s at farm %s from client at version %s") % pRequest->GetLogin() % pRequest->GetFarmNo().ToString() % pRequest->GetClientVersion()).str() );

	//CheckClientCompatibility(pRequest->GetClientVersion());

	LoginResponse* pResponse = new LoginResponse();
	pResponse->WithResult(LoginResult_InvalidLogin);

	auto farmSessionIt = m_FarmSessions.find(pRequest->GetFarmNo());
	if(farmSessionIt != m_FarmSessions.end())
	{
		FarmSession* pFarmSession = farmSessionIt->second;
		
		User* pUser = pFarmSession->Authenticate(pRequest->GetLogin(),pRequest->GetPassword());

		if(pUser != NULL)
		{
			Log::I(Utf8::FromUTF8(session.getClientAddress().string()), L"Login successful");
		
			ClientSessionHolder& sessionHolder = session.createSessionObject<ClientSessionHolder>();
			uint32_t userId = pUser->GetId();
			
			ClientSession* pNewSession = new ClientSession(pFarmSession,userId);
						
			sessionHolder.SetClientSession(pNewSession);
			
			FarmOrganisation organisation;
			const IFarm* pLoginFarm = pFarmSession->GetFarm();
			Farm* pFarm = organisation.CreateFarm(pLoginFarm->GetId());
			pFarm->CopyFrom(pFarmSession->GetFarm());

			PtrEnumerator<IHerd> herdEnum = pFarmSession->EnumHerds();

			IHerd* herd;
			while(herdEnum.hasNext())
			{
				herd = *herdEnum;
				organisation.AddHerd(herd->GetId())->CopyFrom(herd);
				herdEnum.advance();
			};


			ISeason* season;
			PtrEnumerator<ISeason> seasonEnum = pFarmSession->EnumSeasons();
			while (seasonEnum.hasNext())
			{
				season = *seasonEnum;
				organisation.AddSeason(season->GetId())->CopyFrom(season);
				seasonEnum.advance();
			}

			IAgent* agent;
			PtrEnumerator<IAgent> agentEnum = pFarmSession->EnumAgents();
			while (agentEnum.hasNext())
			{
				agent = *agentEnum;
				organisation.AddAgent(agent->GetId())->CopyFrom(agent);
				agentEnum.advance();
			}

			IBankAccount* account;
			PtrEnumerator<IBankAccount> accountEnum = pFarmSession->EnumAccounts();
			while (accountEnum.hasNext())
			{
				account = *accountEnum;
				organisation.AddAccount(account->GetId())->CopyFrom(account);
				accountEnum.advance();
			}

			IClass* cowClass;
			PtrEnumerator<IClass> classEnum = pFarmSession->EnumClasses();
			while (classEnum.hasNext())
			{
				cowClass = *classEnum;
				organisation.AddClass(cowClass->GetId())->CopyFrom(cowClass);
				classEnum.advance();
			}

			IStock* stock;
			PtrEnumerator<IStock> stockEnum = pFarmSession->EnumStocks();
			while (stockEnum.hasNext())
			{
				stock = *stockEnum;
				organisation.AddStock(stock->GetId())->CopyFrom(stock);
				stockEnum.advance();
			}

			SimpleUser simpleUser(pUser->GetId());
			simpleUser.SetFirstName(pUser->GetFirstName());
			simpleUser.SetLastName(pUser->GetLastName());
			simpleUser.SetLogin(pUser->GetLogin());
			pResponse->WithResult(LoginResult_Success);
			pResponse->WithUser(simpleUser);
			pResponse->WithFarmOrganisation(organisation);
		}
		else
		{
			Log::I(Utf8::FromUTF8(session.getClientAddress().string()), L"Login failed");
		}
		
	}

	return LoginResponseObj(pResponse);
}

OpenSeasonResponseObj BKService::OpenSeason(uint32_t seasonId)
{
	ClientSession* pClientSession = AquireClientSession();
	
	ClientSeasonSessionFacade sessionFacade = pClientSession->OpenSeason(seasonId);

	OpenSeasonResponse* pResponse = new OpenSeasonResponse();
	pResponse->WithResult(OpenSeasonResult::Invalid_SeasonId);
	
	if(sessionFacade.IsGood())
	{
		Log::I(Utf8::FromUTF8(RCF::getCurrentRcfSession().getClientAddress().string()),
			(boost::wformat(L"opened season id = %d, observer port = %d") % seasonId % sessionFacade.GetObserverPort()).str());
		pResponse->WithResult(OpenSeasonResult::Open_Successful);
		pResponse->WithSeasonSessionToken(sessionFacade.GetSessionToken());
		pResponse->WithObserverPort(sessionFacade.GetObserverPort());
	}

	return OpenSeasonResponseObj(pResponse);

}

void BKService::CloseSeason(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	pClientSession->CloseSession(sessionToken);
}

FetchManifestObj BKService::BeginFetch(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).BeginFetch();

	FetchManifest* manifest = new FetchManifest();
	
	manifest->HentCount() = handle->HentObjectsCount();	
	manifest->CowCount() = handle->CowObjectsCount();
	manifest->InDocCount() = handle->InDocObjectsCount();
	manifest->BuyDocCount() = handle->BuyDocObjectsCount();
	manifest->MoveDocCount() = handle->MoveDocObjectsCount();
	manifest->SellDocCount() = handle->SellDocObjectsCount();
	manifest->OutDocCount() = handle->OutDocObjectsCount();
	manifest->BuyInvoicesDocCount() = handle->BuyInvoiceObjectsCount();
	manifest->SellInvoicesDocCount() = handle->SellInvoiceObjectsCount();
	
	return FetchManifestObj(manifest);

}

void BKService::EndFetch(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	pClientSession->FindClientSeasonSession(sessionToken).EndFetch();
}

FetchHentsResponseObj BKService::FetchHents(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchHentsResponse* pResponse = new FetchHentsResponse();

	HentServerObject* pSrcHent;
	HentWireObject *pDestHent;
	IHent* pObject;
	for(int i = 0;i < 500;i++)
	{
		pSrcHent = handle->NextHentObject();
		if(!pSrcHent) break;
		
		pObject = pSrcHent->Object();		
		pDestHent = pResponse->AddHent(pObject->GetId());
		pDestHent->CopyFrom(pObject);
		pDestHent->SetIsLocked(pSrcHent->IsLocked());
		

	}
	return FetchHentsResponseObj(pResponse);
}

FetchCowsResponseObj BKService::FetchCows(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchCowsResponse* pResponse = new FetchCowsResponse();

	CowServerObject* pSrc;
	CowWireObject *pDest;
	ICow* pCow;

	for(int i = 0;i < 100;i++)
	{
		pSrc = handle->NextCowObject();
		if(!pSrc) break;
		pCow = pSrc->Object();
		pDest = pResponse->AddCow(pCow->GetId());
		pDest->CopyFrom(pCow);
		pDest->SetIsLocked(pSrc->IsLocked());		
	}
	return FetchCowsResponseObj(pResponse);
}

FetchInDocsResponseObj BKService::FetchInDocs(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchInDocsResponse* pResponse = new FetchInDocsResponse();

	IInDoc* pSrc;
	InDocServerObject* pSrcSO;
	InDocWireObject *pDest;
	for(int i = 0;i < 500;i++)
	{
		pSrcSO = handle->NextInDocObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddDoc(pSrc->GetId());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);		
	}
	return FetchInDocsResponseObj(pResponse);
}

FetchBuyDocsResponseObj BKService::FetchBuyDocs(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchBuyDocsResponse* pResponse = new FetchBuyDocsResponse();

	IBuyDoc* pSrc;
	BuyDocServerObject* pSrcSO;
	BuyDocWireObject *pDest;
	for(int i = 0;i < 100;i++)
	{
		pSrcSO = handle->NextBuyDocObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddDoc(pSrc->GetId());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);		
	}

	return FetchBuyDocsResponseObj(pResponse);
}

FetchMoveDocsResponseObj BKService::FetchMoveDocs(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchMoveDocsResponse* pResponse = new FetchMoveDocsResponse();

	MoveDocServerObject* pSrcSO;
	IMoveDoc* pSrc;
	MoveDocWireObject *pDest;
	
	for(int i = 0;i < 100;i++)
	{
		pSrcSO = handle->NextMoveDocObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddDoc(pSrc->GetId());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);		
	}
	return FetchMoveDocsResponseObj(pResponse);
}

FetchSellDocsResponseObj BKService::FetchSellDocs(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchSellDocsResponse* pResponse = new FetchSellDocsResponse();

	SellDocServerObject* pSrcSO;
	ISellDoc* pSrc;
	SellDocWireObject *pDest;
	
	for(int i = 0;i < 100;i++)
	{
		pSrcSO = handle->NextSellDocObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddDoc(pSrc->GetId());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);
	}
	return FetchSellDocsResponseObj(pResponse);
}

FetchOutDocsResponseObj BKService::FetchOutDocs(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchOutDocsResponse* pResponse = new FetchOutDocsResponse();

	OutDocServerObject* pSrcSO;
	IOutDoc* pSrc;
	OutDocWireObject *pDest;
	
	for(int i = 0;i < 300;i++)
	{
		pSrcSO = handle->NextOutDocObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddDoc(pSrc->GetId());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);
	}
	return FetchOutDocsResponseObj(pResponse);
}

FetchInvoicesResponseObj BKService::FetchBuyInvoices(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchInvoicesResponse* pResponse = new FetchInvoicesResponse();

	BuyInvoiceServerObject* pSrcSO;
	IBuyInvoice* pSrc;
	WireInvoice *pDest;
	
	for(int i = 0;i < 100;i++)
	{
		pSrcSO = handle->NextBuyInvoiceObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddInvoice(pSrc->GetId(),pSrc->GetInvoiceScope());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);		
	}
	return FetchInvoicesResponseObj(pResponse);
}

FetchInvoicesResponseObj BKService::FetchSellInvoices(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();

	FetchHandle handle = pClientSession->FindClientSeasonSession(sessionToken).GetFetchHandle();

	FetchInvoicesResponse* pResponse = new FetchInvoicesResponse();

	SellInvoiceServerObject* pSrcSO;
	ISellInvoice* pSrc;
	WireInvoice *pDest;
	
	for(int i = 0;i < 100;i++)
	{
		pSrcSO = handle->NextSellInvoiceObject();
		if(!pSrcSO) break;
		pSrc = pSrcSO->Object();
		pDest = pResponse->AddInvoice(pSrc->GetId(),pSrc->GetInvoiceScope());
		pDest->SetIsLocked(pSrcSO->IsLocked());
		pDest->CopyFrom(pSrc);		
	}
	return FetchInvoicesResponseObj(pResponse);
}


void BKService::ThrowIfInvalidSeasonSession(ClientSeasonSessionFacade sessionFacade)
{
	if(!sessionFacade.IsGood())
		BOOST_THROW_EXCEPTION(SessionException(L"invalid session token"));
}

ClientSeasonSessionFacade BKService::AquireClientSeasonSession(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();
	ClientSeasonSessionFacade session = pClientSession->FindClientSeasonSession(sessionToken);
	ThrowIfInvalidSeasonSession(session);
	return session;
}

uint32_t BKService::InsertHent(uint32_t sessionToken,HentRequestObj hentToInsert)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.InsertHent(hentToInsert->GetHent());
}

void BKService::UpdateHent(uint32_t sessionToken,HentRequestObj hentToUpdate)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UpdateHent(hentToUpdate->GetHent());
}

void  BKService::DeleteHent(uint32_t sessionToken,uint32_t hentId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.DeleteHent(hentId);
}

void BKService::LockHent(uint32_t sessionToken,uint32_t hentId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.LockHent(hentId);
}

void BKService::UnlockHent(uint32_t sessionToken,uint32_t hentId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UnlockHent(hentId);
}

void BKService::LockCow(uint32_t sessionToken,uint32_t cowId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);	
	session.LockCow(cowId);
}

void BKService::UnlockCow(uint32_t sessionToken,uint32_t cowId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UnlockCow(cowId);
}

void BKService::UpdateCow(uint32_t sessionToken,CowRequestObj cow)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UpdateCow(cow->GetCowId(),cow->GetDetails());
}

uint32_t BKService::OpenNewInDocTransaction(uint32_t sessionToken,uint32_t herdId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewInDocTransaction(herdId)->GetId();
}

uint32_t BKService::OpenEditInDocTransaction(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditInDocTransaction(docId)->GetId();
}

TransactionResponseObj BKService::ExecuteOperation(uint32_t sessionToken,TransactionRequestObj rqObj)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	boost::any operationResult = session.ExecuteOperation(rqObj->GetTransactionId(),rqObj->GetOperation(),rqObj->GetParameters());
	TransactionResponse* response = new TransactionResponse();
	response->WithResult(operationResult);
	return TransactionResponseObj(response);	
}

void BKService::LockInDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.LockInDocWithCows(docId);	
}

void BKService::UnlockInDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.UnlockInDoc(docId);	
}

void BKService::DeleteInDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.DeleteInDoc(docId);	
}

void BKService::CommitTransaction(uint32_t sessionToken,uint32_t transactionId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.CommitTransaction(transactionId);	
}

void BKService::AbortTransaction(uint32_t sessionToken,uint32_t transactionId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.AbortTransaction(transactionId);	
}
	
uint32_t BKService::OpenNewMoveDocTransaction(uint32_t sessionToken,uint32_t srcHerdId,uint32_t dstHerdId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewMoveDocTransaction(srcHerdId,dstHerdId)->GetId();
}

void BKService::LockMoveDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.LockMoveDoc(docId);
}

void BKService::UnlockMoveDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.UnlockMoveDoc(docId);
}

void BKService::DeleteMoveDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.DeleteMoveDoc(docId);
}

uint32_t BKService::OpenNewSellDocTransaction(uint32_t sessionToken,uint32_t herdId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewSellDocTransaction(herdId)->GetId();
}

void BKService::LockSellDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.LockSellDoc(docId);
}

void BKService::UnlockSellDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.UnlockSellDoc(docId);
}

void BKService::DeleteSellDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.DeleteSellDoc(docId);
}

uint32_t BKService::OpenNewOutDocTransaction(uint32_t sessionToken,uint32_t herdId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewOutDocTransaction(herdId)->GetId();
}

void BKService::LockOutDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.LockOutDoc(docId);
}

void BKService::UnlockOutDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UnlockOutDoc(docId);
}

void BKService::DeleteOutDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.DeleteOutDoc(docId);
}

uint32_t BKService::OpenNewBuyDocTransaction(uint32_t sessionToken,uint32_t herdId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewBuyDocTransaction(herdId)->GetId();
}

void BKService::LockBuyDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.LockBuyDoc(docId);
}

void BKService::UnlockBuyDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UnlockBuyDoc(docId);
}

void BKService::DeleteBuyDoc(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.DeleteBuyDoc(docId);
}

uint32_t BKService::OpenEditMoveDocTransaction(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditMoveDocTransaction(docId)->GetId();
}

uint32_t BKService::OpenEditSellDocTransaction(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditSellDocTransaction(docId)->GetId();
}

uint32_t BKService::OpenEditOutDocTransaction(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditOutDocTransaction(docId)->GetId();
}

uint32_t BKService::OpenEditBuyDocTransaction(uint32_t sessionToken,uint32_t docId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditBuyDocTransaction(docId)->GetId();
}

uint32_t BKService::OpenNewBuyInvoiceTransaction(uint32_t sessionToken,InvoiceScope scope)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewBuyInvoiceTransaction(scope)->GetId();
}

uint32_t BKService::OpenEditBuyInvoiceTransaction(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditBuyInvoiceTransaction(invoiceId)->GetId();
}

void BKService::DeleteBuyInvoice(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.DeleteBuyInvoice(invoiceId);
}

uint32_t BKService::OpenNewSellInvoiceTransaction(uint32_t sessionToken,InvoiceScope scope)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenNewSellInvoiceTransaction(scope)->GetId();
}

uint32_t BKService::OpenEditSellInvoiceTransaction(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	return session.OpenEditSellInvoiceTransaction(invoiceId)->GetId();
}

void BKService::DeleteSellInvoice(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.DeleteSellInvoice(invoiceId);
}

void BKService::LockSellInvoice(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.LockSellInvoice(invoiceId);

}

void BKService::UnlockSellInvoice(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UnlockSellInvoice(invoiceId);
}

void BKService::LockBuyInvoice(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.LockBuyInvoice(invoiceId);
}

void BKService::UnlockBuyInvoice(uint32_t sessionToken,uint32_t invoiceId)
{
	ClientSeasonSessionFacade session  = AquireClientSeasonSession(sessionToken);
	session.UnlockBuyInvoice(invoiceId);
}

SettingsObj BKService::FetchSettings(uint32_t sessionToken)
{
	ClientSession* pClientSession = AquireClientSession();	
	UserSettings settings = pClientSession->GetSettings();

	SettingsObj responseObj(new Settings());

	PtrEnumerator<ISetting> settingEnum = settings.EnumSettings();
	ISetting* setting;
	while(settingEnum.hasNext())
	{
		setting = *settingEnum;
		responseObj->AddSetting(setting);
		settingEnum.advance();
	}

	return responseObj;
}

void BKService::SaveSettings(uint32_t sessionToken,SettingsObj settingsRq)
{
	ClientSession* pClientSession = AquireClientSession();
	
	ISetting* setting;
	UserSettings settings;
	PtrEnumerator<ISetting> settingEnum = settingsRq->EnumSettings();
	while (settingEnum.hasNext())
	{
		setting = *settingEnum;
		settings.SetSetting(setting->GetName(), setting->GetValue());
		settingEnum.advance();
	}

	pClientSession->SaveSettings(settings);
}

void BKService::MarkPaidSellInvoice(uint32_t sessionToken, MarkInvoicePaidRequestObj request)
{
}

void BKService::MarkPaidBuyInvoice(uint32_t sessionToken, MarkInvoicePaidRequestObj request)
{
}

FetchPrintProfilesResponseObj BKService::FetchProfiles()
{
	ClientSession* pClientSession = AquireClientSession();
	PrintProfiles profiles = pClientSession->FetchPrintProfiles();
	
	FetchPrintProfilesResponse* response = new FetchPrintProfilesResponse();

	BOOST_FOREACH(PrintProfile* profile, profiles)
	{
		response->AddProfile(profile->GetProfileId())->CopyFrom(profile);
	}	
	return FetchPrintProfilesResponseObj(response);
}

void BKService::UpdatePrintProfile(PrintProfileRequestObj profile)
{
	ClientSession* pClientSession = AquireClientSession();
	pClientSession->UpdatePrintProfile(profile->GetProfile());
}

uint32_t BKService::NewPrintProfile(PrintProfileRequestObj profile)
{
	ClientSession* pClientSession = AquireClientSession();
	return pClientSession->NewPrintProfile(profile->GetProfile());
}

void BKService::DeletePrintProfile(uint32_t profileId)
{
	ClientSession* pClientSession = AquireClientSession();
	pClientSession->DeletePrintProfile(profileId);
}


FetchPassportRecognitionProfilesResponseObj BKService::FetchPassportRecognitionProfiles()
{
	ClientSession* pClientSession = AquireClientSession();
	PassportRecognitionProfiles profiles = pClientSession->FetchPassportRecognitionProfiles();

	FetchPassportRecognitionProfilesResponse* response = new FetchPassportRecognitionProfilesResponse();

	BOOST_FOREACH(PassportRecognitionProfile* profile, profiles)
	{
		response->AddProfile(profile->GetId())->CopyFrom(profile);
	}
	return FetchPassportRecognitionProfilesResponseObj(response);
}

void BKService::UpdatePassportRecognitionProfile(PassportRecognitionProfileRequestObj rq)
{
	ClientSession* pClientSession = AquireClientSession();
	pClientSession->UpdatePassportRecognitionProfile(rq->GetProfile());
}

uint32_t BKService::NewPassportRecognitionProfile(PassportRecognitionProfileRequestObj rq)
{
	ClientSession* pClientSession = AquireClientSession();
	return pClientSession->NewPassportRecognitionProfile(rq->GetProfile());
}

void BKService::DeletePassportRecognitionProfile(uint32_t profileId)
{
	ClientSession* pClientSession = AquireClientSession();
	pClientSession->DeletePassportRecognitionProfile(profileId);
}


FetchDeductionDefinitionsResponseObj BKService::FetchDeductionDefinitions()
{
	ClientSession* pClientSession = AquireClientSession();
	DeductionDefinitions definitions = pClientSession->FetchDeductionDefinitions();

	FetchDeductionDefinitionsResponse* response = new FetchDeductionDefinitionsResponse();

	BOOST_FOREACH(DeductionDefinition* definition, definitions)
	{
		response->AddDefinition(definition->GetId())->CopyFrom(definition);
	}
	return FetchDeductionDefinitionsResponseObj(response);
}

void BKService::UpdateDeductionDefinition(DeductionDefinitionRequestObj rq)
{
	ClientSession* pClientSession = AquireClientSession();
	return pClientSession->UpdateDeductionDefinition(rq->GetDefinition());
}

uint32_t BKService::NewDeductionDefinition(DeductionDefinitionRequestObj rq)
{
	ClientSession* pClientSession = AquireClientSession();
	return pClientSession->NewDeductionDefinition(rq->GetDefinition());
}

void BKService::DeleteDeductionDefinition(uint32_t deductionId)
{
	ClientSession* pClientSession = AquireClientSession();
	return pClientSession->DeleteDeductionDefinition(deductionId);
}


void BKService::OpenPurchaseRepository(uint32_t sessionToken)
{
	ClientSeasonSessionFacade session = AquireClientSeasonSession(sessionToken);
	session.OpenPurchaseRepository();
}

int BKService::QueryPurchases(uint32_t sessionToken,PurchaseQueryObj rq)
{
	ClientSeasonSessionFacade session = AquireClientSeasonSession(sessionToken);
	return session.QueryPurchases(rq->GetStartDt(),rq->GetEndDt());
}

uint32_t BKService::OpenDownloadPurchaseTransaction(uint32_t sessionToken, std::wstring purchaseId)
{
	ClientSeasonSessionFacade session = AquireClientSeasonSession(sessionToken);
	return session.OpenDownloadPurchaseTransaction(purchaseId)->GetId();
}

void BKService::ClosePurchaseRepository(uint32_t sessionToken)
{
	ClientSeasonSessionFacade session = AquireClientSeasonSession(sessionToken);
	session.ClosePurchaseRepository();
}

FetchPurchaseHeadersResponseObj BKService::FetchPurchaseHeaders(uint32_t sessionToken)
{
	ClientSeasonSessionFacade session = AquireClientSeasonSession(sessionToken);

	FetchPurchaseHeadersResponse* rs = new FetchPurchaseHeadersResponse();
	PurchaseHeaders purchasesChunk = session.FetchPurchaseHeaders();

	BOOST_FOREACH(IPurchaseHeader* purchaseHeader, purchasesChunk)
	{
		rs->Add(purchaseHeader->GetId())->CopyFrom(purchaseHeader);
	}

	return FetchPurchaseHeadersResponseObj(rs);
}

FetchPurchaseResponseObj BKService::FetchPurchase(uint32_t sessionToken, std::wstring purchaseId)
{
	ClientSeasonSessionFacade session = AquireClientSeasonSession(sessionToken);

	FetchPurchaseResponse* rs = new FetchPurchaseResponse();
	boost::optional<Purchase> purchase = session.FetchPurchase(purchaseId);

	if (purchase)
	{
		rs->Set(purchase.get_ptr());
	}
	return FetchPurchaseResponseObj(rs);
}
