#pragma once

#include "object/HentServerObject.h"
#include <data/IStock.h>
#include <data/IClass.h>
#include "object/CowServerObject.h"
#include <data/IInDoc.h>
#include <data/IOutDoc.h>
#include <data/IBuyDoc.h>
#include <data/ISellDoc.h>
#include <data/IMoveDoc.h>
#include <data/IBuyInvoice.h>
#include <data/ISellInvoice.h>
#include <data/IAgent.h>
#include <arrays/NewPtrFlushArray.h>
#include <arrays/Enumerator.h>
#include "transaction/IClientTransaction.h"
#include "IFetchHandle.h"
#include <data/DocKey.h>
#include <data/InvoiceKey.h>
#include <data/repo/PurchaseHeaders.h>
#include <data/repo/Purchase.h>
#include "settings/UserSettings.h"

class ClientSeasonSession;

class ClientSeasonSessionFacade
{
public:
	ClientSeasonSessionFacade(ClientSeasonSession* pSession);
	ClientSeasonSessionFacade(const ClientSeasonSessionFacade& src);
	ClientSeasonSessionFacade& operator=(const ClientSeasonSessionFacade& right);

	uint32_t GetSessionToken() const;
	uint32_t GetSeasonId() const;	
	uint16_t GetObserverPort() const;
	bool IsGood() const;

	void LockHent(uint32_t hentId);
	void UnlockHent(uint32_t hentId);
	void LockCow(uint32_t cowId);
	void UnlockCow(uint32_t cowId);

	//void LockInDoc(uint32_t docId);
	void LockInDocWithCows(uint32_t docId);
	void UnlockInDoc(uint32_t docId);
	void DeleteInDoc(uint32_t docId);

	void LockBuyDoc(uint32_t docId);
	void UnlockBuyDoc(uint32_t docId);
	void DeleteBuyDoc(uint32_t docId);

	void LockMoveDoc(uint32_t docId);
	void UnlockMoveDoc(uint32_t docId);
	void DeleteMoveDoc(uint32_t docId);

	void LockOutDoc(uint32_t docId);
	void UnlockOutDoc(uint32_t docId);
	void DeleteOutDoc(uint32_t docId);

	void LockSellDoc(uint32_t docId);
	void UnlockSellDoc(uint32_t docId);	
	void DeleteSellDoc(uint32_t docId);
	
	FetchHandle BeginFetch();
	FetchHandle GetFetchHandle();
	void EndFetch();

	void UpdateCow(uint32_t cowId,const CowDetails& details);

	IClientTransaction* OpenNewInDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditInDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewMoveDocTransaction(uint32_t srcHerdId,uint32_t dstHerdId);
	IClientTransaction* OpenEditMoveDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewSellDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditSellDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewBuyDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditBuyDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewOutDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditOutDocTransaction(uint32_t docId);

	IClientTransaction* FindDocTransaction(uint32_t transactionId);

	void LockBuyInvoice(uint32_t invoiceId);
	void UnlockBuyInvoice(uint32_t invoiceId);
	IClientTransaction* OpenNewBuyInvoiceTransaction(InvoiceScope scope);
	IClientTransaction* OpenEditBuyInvoiceTransaction(uint32_t invoiceId);
	void DeleteBuyInvoice(uint32_t invoiceId);	

	void LockSellInvoice(uint32_t invoiceId);
	void UnlockSellInvoice(uint32_t invoiceId);
	IClientTransaction* OpenNewSellInvoiceTransaction(InvoiceScope scope);
	IClientTransaction* OpenEditSellInvoiceTransaction(uint32_t invoiceId);
	void DeleteSellInvoice(uint32_t invoiceId);

	IClientTransaction* FindInvoiceTransaction(uint32_t transactionId);

	void CommitTransaction(uint32_t transactionId);
	void AbortTransaction(uint32_t transactionId);
	boost::any ExecuteOperation(uint32_t transactionId,uint32_t operation,const OperationArguments& args);
		
	uint32_t InsertHent(IHent* pHent);
	void UpdateHent(IHent* pHent);
	void DeleteHent(uint32_t hentId);
	void Close();

	void OpenPurchaseRepository();
	int QueryPurchases(const DateTime& startDt,const DateTime& endDt);
	PurchaseHeaders FetchPurchaseHeaders();
	boost::optional<Purchase> FetchPurchase(const std::wstring& purchaseId);
	IClientTransaction* OpenDownloadPurchaseTransaction(const std::wstring& purchaseId);
	void ClosePurchaseRepository();


protected:
	void ThrowIfNoDelegate() const;
	
private:
	ClientSeasonSession* m_pSessionDelegate;

};
