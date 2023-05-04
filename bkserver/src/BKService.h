#pragma once

#include <service/messages/LoginRequest.h>
#include <service/messages/LoginResponse.h>
#include <arrays/NewPtrFlushArray.h>
#include <service/messages/OpenSeasonResponse.h>
#include <service/messages/FetchHentsResponse.h>
#include <service/messages/HentRequest.h>
#include <service/messages/FetchCowsResponse.h>
#include <service/messages/FetchClassesResponse.h>
#include <service/messages/FetchStocksResponse.h>
#include <service/messages/FetchDocsResponses.h>
#include <service/messages/FetchInvoicesResponse.h>
#include <service/messages/CowRequest.h>
#include <service/messages/TransactionRequest.h>
#include <service/messages/TransactionResponse.h>
#include <service/messages/FetchManifest.h>
#include "FarmSession.h"
#include <data/impl/Hent.h>
#include <service/messages/Settings.h>
#include <service/messages/MarkInvoicePaidRequest.h>
#include <service/messages/FetchPrintProfilesResponse.h>
#include <service/messages/PrintProfileRequest.h>
#include <service/messages/FetchPassportRecognitionProfilesResponse.h>
#include <service/messages/PassportRecognitionProfileRequest.h>
#include <service/messages/FetchDeductionDefinitionsResponse.h>
#include <service/messages/DeductionDefinitionRequest.h>
#include <service/messages/PurchaseQuery.h>
#include <service/messages/FetchPurchaseResponse.h>
#include <service/messages/FetchPurchaseHeadersResponse.h>

#define REQUIRED_CLIENT_VERSION "*.*.>1.*"

class ClientSessionHolder
{
public:
	ClientSessionHolder();
	void SetClientSession(ClientSession* pSession);
	ClientSession* GetClientSession() const;
	~ClientSessionHolder();
private:
	ClientSession* m_pSession;
};

class BKServiceConstructor;
class BKServiceDestructor;

class BKService
{
private:
	ClientSession* AquireClientSession();
	void ThrowIfInvalidSeasonSession(ClientSeasonSessionFacade sessionFacade);
	ClientSeasonSessionFacade AquireClientSeasonSession(uint32_t sessionToken);
	void ThrowTransactionNotFound();
public:
	BKService();
    LoginResponseObj Login(LoginRequestObj request);
	OpenSeasonResponseObj OpenSeason(uint32_t seasonId);
	void CloseSeason(uint32_t sessionToken);
	FetchManifestObj BeginFetch(uint32_t sessionToken);

	FetchHentsResponseObj FetchHents(uint32_t sessionToken);	
	FetchCowsResponseObj FetchCows(uint32_t sessionToken);
	FetchInDocsResponseObj FetchInDocs(uint32_t sessionToken);
	FetchBuyDocsResponseObj FetchBuyDocs(uint32_t sessionToken);
	FetchMoveDocsResponseObj FetchMoveDocs(uint32_t sessionToken);
	FetchSellDocsResponseObj FetchSellDocs(uint32_t sessionToken);
	FetchOutDocsResponseObj FetchOutDocs(uint32_t sessionToken);
	FetchInvoicesResponseObj FetchBuyInvoices(uint32_t sessionToken);
	FetchInvoicesResponseObj FetchSellInvoices(uint32_t sessionToken);

	uint32_t OpenNewInDocTransaction(uint32_t sessionToken,uint32_t herdId);
	uint32_t OpenEditInDocTransaction(uint32_t sessionToken,uint32_t docId);
	void LockInDoc(uint32_t sessionToken,uint32_t docId);
	void UnlockInDoc(uint32_t sessionToken,uint32_t docId);
	void DeleteInDoc(uint32_t sessionToken,uint32_t docId);

	uint32_t OpenNewMoveDocTransaction(uint32_t sessionToken,uint32_t srcHerdId,uint32_t dstHerdId);	
	uint32_t OpenEditMoveDocTransaction(uint32_t sessionToken,uint32_t docId);
	void LockMoveDoc(uint32_t sessionToken,uint32_t docId);
	void UnlockMoveDoc(uint32_t sessionToken,uint32_t docId);
	void DeleteMoveDoc(uint32_t sessionToken,uint32_t docId);

	uint32_t OpenNewSellDocTransaction(uint32_t sessionToken,uint32_t herdId);
	uint32_t OpenEditSellDocTransaction(uint32_t sessionToken,uint32_t docId);
	void LockSellDoc(uint32_t sessionToken,uint32_t docId);
	void UnlockSellDoc(uint32_t sessionToken,uint32_t docId);
	void DeleteSellDoc(uint32_t sessionToken,uint32_t docId);

	uint32_t OpenNewOutDocTransaction(uint32_t sessionToken,uint32_t herdId);
	uint32_t OpenEditOutDocTransaction(uint32_t sessionToken,uint32_t docId);
	void LockOutDoc(uint32_t sessionToken,uint32_t docId);
	void UnlockOutDoc(uint32_t sessionToken,uint32_t docId);
	void DeleteOutDoc(uint32_t sessionToken,uint32_t docId);

	uint32_t OpenNewBuyDocTransaction(uint32_t sessionToken,uint32_t herdId);
	uint32_t OpenEditBuyDocTransaction(uint32_t sessionToken,uint32_t docId);
	void LockBuyDoc(uint32_t sessionToken,uint32_t docId);
	void UnlockBuyDoc(uint32_t sessionToken,uint32_t docId);
	void DeleteBuyDoc(uint32_t sessionToken,uint32_t docId);

	void LockBuyInvoice(uint32_t sessionToken,uint32_t invoiceId);
	void UnlockBuyInvoice(uint32_t sessionToken,uint32_t invoiceId);

	uint32_t OpenNewBuyInvoiceTransaction(uint32_t sessionToken,InvoiceScope scope);
	uint32_t OpenEditBuyInvoiceTransaction(uint32_t sessionToken,uint32_t invoiceId);
	void DeleteBuyInvoice(uint32_t sessionToken,uint32_t invoiceId);
	

	void LockSellInvoice(uint32_t sessionToken,uint32_t invoiceId);
	void UnlockSellInvoice(uint32_t sessionToken,uint32_t invoiceId);

	uint32_t OpenNewSellInvoiceTransaction(uint32_t sessionToken,InvoiceScope scope);
	uint32_t OpenEditSellInvoiceTransaction(uint32_t sessionToken,uint32_t invoiceId);
	void DeleteSellInvoice(uint32_t sessionToken,uint32_t invoiceId);

	TransactionResponseObj ExecuteOperation(uint32_t sessionToken,TransactionRequestObj);
	void CommitTransaction(uint32_t sessionToken,uint32_t transactionId);
	void AbortTransaction(uint32_t sessionToken,uint32_t transactionId);
	
	void EndFetch(uint32_t sessionToken);

	void LockHent(uint32_t sessionToken,uint32_t hentId);
	void UnlockHent(uint32_t sessionToken,uint32_t hentId);

	uint32_t InsertHent(uint32_t sessionToken,HentRequestObj hentToInsert);
	void UpdateHent(uint32_t sessionToken,HentRequestObj hentToUpdate);
	void DeleteHent(uint32_t sessionToken,uint32_t hentId);

	void LockCow(uint32_t sessionToken,uint32_t cowId);
	void UnlockCow(uint32_t sessionToken,uint32_t cowId);
	void UpdateCow(uint32_t sessionToken,CowRequestObj cow);

	SettingsObj FetchSettings(uint32_t sessionToken);
	void SaveSettings(uint32_t sessionToken,SettingsObj settings);

	FetchPrintProfilesResponseObj FetchProfiles();
	void UpdatePrintProfile(PrintProfileRequestObj profile);
	uint32_t NewPrintProfile(PrintProfileRequestObj profile);
	void DeletePrintProfile(uint32_t profileId);

	void MarkPaidSellInvoice(uint32_t sessionToken, MarkInvoicePaidRequestObj request); 
	void MarkPaidBuyInvoice(uint32_t sessionToken, MarkInvoicePaidRequestObj request); 

	FetchPassportRecognitionProfilesResponseObj FetchPassportRecognitionProfiles();
	void UpdatePassportRecognitionProfile(PassportRecognitionProfileRequestObj rq);
	uint32_t NewPassportRecognitionProfile(PassportRecognitionProfileRequestObj rq);
	void DeletePassportRecognitionProfile(uint32_t profileId);

	FetchDeductionDefinitionsResponseObj FetchDeductionDefinitions();
	void UpdateDeductionDefinition(DeductionDefinitionRequestObj rq);
	uint32_t NewDeductionDefinition(DeductionDefinitionRequestObj rq);
	void DeleteDeductionDefinition(uint32_t deductionId);

	void OpenPurchaseRepository(uint32_t sessionToken);
	int QueryPurchases(uint32_t sessionToken, PurchaseQueryObj rq);
	FetchPurchaseResponseObj FetchPurchase(uint32_t sessionToken, std::wstring purchaseId);
	FetchPurchaseHeadersResponseObj FetchPurchaseHeaders(uint32_t sessionToken);
	uint32_t OpenDownloadPurchaseTransaction(uint32_t sessionToken, std::wstring purchaseId);
	void ClosePurchaseRepository(uint32_t sessionToken);

	
private:
	std::map<FarmNo,FarmSession*> m_FarmSessions;
	CloudPublisher* m_publisher;
	
	friend class BKServiceConstructor;
	friend class BKServiceDestructor;
};
