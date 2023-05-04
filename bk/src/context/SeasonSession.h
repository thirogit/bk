#ifndef __SEASONSESSION_H__
#define __SEASONSESSION_H__

#include "SeasonObserver.h"
#include "arrays\Enumerator.h"
#include "transaction\InDocTransaction.h"
#include "transaction\BuyDocTransaction.h"
#include "transaction\MoveDocTransaction.h"
#include "transaction\SellDocTransaction.h"
#include "transaction\OutDocTransaction.h"
#include "transaction\InvoiceCowsTransaction.h"
#include "transaction\InvoiceCowsTransaction.h"
#include "transaction\InvoiceDocsTransaction.h"
#include "object\HentClientObject.h"
#include "object\CowClientObject.h"
#include "object\InDocClientObject.h"
#include "object\BuyDocClientObject.h"
#include "object\MoveDocClientObject.h"
#include "object\SellDocClientObject.h"
#include "object\OutDocClientObject.h"
#include "object\BuyInvoiceClientObject.h"
#include "object\SellInvoiceClientObject.h"
#include "HerdObserver.h"
#include <logic\herdregistry\HerdRegistry.h>
#include <data\IClass.h>
#include <data\IAgent.h>
#include <data\IStock.h>
#include <data\IHerd.h>
#include <data\IFarm.h>
#include <data\ISeason.h>
#include <data\IBankAccount.h>
#include <logic\cowtrail\ICowTrail.h>
#include <boost\range\any_range.hpp>
#include "UserSettings.h"

#include <data\repo\PurchaseHeaders.h>
#include "transaction\DownloadPurchaseTransaction.h"
#include <progress\ComplexProgress.h>

class XContext;

class SeasonSession
{
public:
	virtual XContext* GetContext() = 0;
	virtual ISeason* GetSeason() = 0;
	virtual void BeginFetch() = 0;
	virtual void EndFetch()  = 0;
	virtual void DoFetch(ComplexProgress* progress) = 0;

	virtual uint32_t InsertHent(const IHent* pHentToInsert) = 0;
	virtual void UpdateHent(const IHent* pHentToUpdate) = 0;
	virtual void DeleteHent(uint32_t hentId) = 0;
	virtual void LockHent(uint32_t hentId) = 0 ;
	virtual void UnlockHent(uint32_t hentId) = 0;
	virtual PtrEnumerator<HentClientObject> EnumHents() = 0;

	virtual void AddSeasonObserver(SeasonObserver* pObserver) = 0;
	virtual void RemoveSeasonObserver(SeasonObserver* pObserver) = 0;

	virtual void AddHerdObserver(uint32_t herdId,HerdObserver* pObserver) = 0;
	virtual void RemoveHerdObserver(uint32_t herdId,HerdObserver* pObserver) = 0;

	virtual HentClientObject* GetHent(uint32_t hentId) = 0;
	virtual HentClientObject* GetHent(const HentNo& hent) = 0;
	virtual CowClientObject* GetCow(uint32_t cowId) = 0;
	virtual HerdRegistry* GetHerdRegistry(uint32_t herdId) = 0;
	virtual IHerd* GetHerd(uint32_t herdId) = 0;
	virtual IHerd* GetHerdByIndex(int herdIndex) = 0;
	virtual IClass* GetClass(uint32_t classId) = 0;
	virtual IClass* GetClassByCd(const std::wstring& classCd) = 0;
	virtual IStock* GetStock(uint32_t stockId) = 0;
	virtual ICowTrail* GetCowTrail(uint32_t cowId) = 0; 
	virtual PtrEnumerator<IStock> EnumStocks() = 0;
	virtual PtrEnumerator<IClass> EnumClasses() = 0;
	virtual PtrEnumerator<IAgent> EnumAgents() = 0;
	virtual IAgent* GetAgent(uint32_t agentId) = 0;
	virtual IBankAccount* GetBankAccount(uint32_t accountId) = 0;
	virtual PtrEnumerator<IBankAccount> EnumBankAccounts() = 0;
	virtual IFarm* GetFarm() = 0;
	virtual InDocClientObject* GetInDoc(uint32_t docId) = 0;

	virtual BuyDocClientObject* GetBuyDoc(uint32_t docId) = 0;
	virtual PtrEnumerator<BuyDocClientObject> EnumBuyDocs() = 0;
	virtual PtrEnumerator<SellDocClientObject> EnumSellDocs() = 0;	

	virtual MoveDocClientObject* GetMoveDoc(uint32_t docId) = 0;
	virtual SellDocClientObject* GetSellDoc(uint32_t docId) = 0;	
	virtual OutDocClientObject* GetOutDoc(uint32_t docId) = 0;

	virtual PtrEnumerator<IHerd> EnumHerds() = 0;

	virtual void LockCow(uint32_t cowId) = 0 ;
	virtual void UnlockCow(uint32_t cowId) = 0;
	virtual void UpdateCow(uint32_t cowId,const CowDetails& details) = 0 ;

	virtual void LockSellDoc(uint32_t docId) = 0;
	virtual void UnlockSellDoc(uint32_t docId) = 0;	
	virtual void DeleteSellDoc(uint32_t docId) = 0;

	virtual void LockMoveDoc(uint32_t docId) = 0;
	virtual void UnlockMoveDoc(uint32_t docId) = 0;	
	virtual void DeleteMoveDoc(uint32_t docId) = 0;

	virtual void LockOutDoc(uint32_t docId) = 0;
	virtual void UnlockOutDoc(uint32_t docId) = 0;	
	virtual void DeleteOutDoc(uint32_t docId) = 0;

	virtual void LockBuyDoc(uint32_t docId) = 0;
	virtual void UnlockBuyDoc(uint32_t docId) = 0;	
	virtual void DeleteBuyDoc(uint32_t docId) = 0;

	virtual void LockInDoc(uint32_t docId) = 0;
	virtual void UnlockInDoc(uint32_t docId) = 0;	
	virtual void DeleteInDoc(uint32_t docId) = 0;

	virtual void LockBuyInvoice(uint32_t invoiceId) = 0;
	virtual void UnlockBuyInvoice(uint32_t invoiceId) = 0;
	virtual void DeleteBuyInvoice(uint32_t invoiceId) = 0;
	virtual BuyInvoiceClientObject* GetBuyInvoice(uint32_t invoiceId) = 0;
	virtual PtrEnumerator<BuyInvoiceClientObject> EnumBuyInvoices() = 0;

	virtual void LockSellInvoice(uint32_t invoiceId) = 0;
	virtual void UnlockSellInvoice(uint32_t invoiceId) = 0;
	virtual void DeleteSellInvoice(uint32_t invoiceId) = 0;
	virtual SellInvoiceClientObject* GetSellInvoice(uint32_t invoiceId) = 0;
	virtual PtrEnumerator<SellInvoiceClientObject> EnumSellInvoices() = 0;
	
	virtual void Close() = 0;

	virtual InDocTransactionPtr NewInDoc(uint32_t herdId) = 0;
	virtual InDocTransactionPtr EditInDoc(uint32_t docId) = 0;	
	virtual BuyDocTransactionPtr NewBuyDoc(uint32_t herdId) = 0;
	virtual BuyDocTransactionPtr EditBuyDoc(uint32_t docId) = 0;	
	virtual MoveDocTransactionPtr NewMoveDoc(uint32_t fromHerdId,uint32_t toHerdId) = 0;
	virtual MoveDocTransactionPtr EditMoveDoc(uint32_t docId) = 0;	
	virtual SellDocTransactionPtr NewSellDoc(uint32_t herdId) = 0;
	virtual SellDocTransactionPtr EditSellDoc(uint32_t docId) = 0;	
	virtual OutDocTransactionPtr NewOutDoc(uint32_t herdId) = 0;
	virtual OutDocTransactionPtr EditOutDoc(uint32_t docId) = 0;	
	virtual InvoiceDocsTransactionPtr NewBuyInvoiceDocs() = 0;
	virtual InvoiceDocsTransactionPtr EditBuyInvoiceDocs(uint32_t invoiceId) = 0;
	virtual InvoiceCowsTransactionPtr NewBuyInvoiceCows() = 0;
	virtual InvoiceCowsTransactionPtr EditBuyInvoiceCows(uint32_t invoiceId) = 0;
	
	virtual InvoiceDocsTransactionPtr NewSellInvoiceDocs() = 0;
	virtual InvoiceDocsTransactionPtr EditSellInvoiceDocs(uint32_t invoiceId) = 0;
	virtual InvoiceCowsTransactionPtr NewSellInvoiceCows() = 0;
	virtual InvoiceCowsTransactionPtr EditSellInvoiceCows(uint32_t invoiceId) = 0;

	virtual UserSettings* GetUserSettings() = 0;	
	
	virtual void OpenPurchaseRepository() = 0;
	virtual int QueryPurchases(const DateTime& startDt,const DateTime& endDt) = 0;
	virtual PurchaseHeaders FetchPurchaseHeaders() = 0;
	virtual boost::optional<Purchase> FetchPurchase(const std::wstring& purchaseId) = 0;
	virtual DownloadPurchaseTransactionPtr OpenDownloadPurchaseTransaction(const std::wstring& purchaseId) = 0;
	virtual void ClosePurchaseRepository() = 0;
	
};

#endif