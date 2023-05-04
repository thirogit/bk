#ifndef __SEASONSESSIONIMPL_H__
#define __SEASONSESSIONIMPL_H__

#include "SeasonSession.h"
#include <logic\cowtrail\CowTracker.h>
#include "NotificationSubscriber.h"
#include "NotificationRelayImpl.h"
#include "UserSettingsImpl.h"
#include "Observerable.h"
#include "SeasonObserver.h"
#include <data\impl\Agent.h>
#include <data\impl\Class.h>
#include <data\impl\Stock.h>
#include <data\impl\Herd.h>
#include <unordered_map>
#include <boost\any.hpp>
#include <configuration\Setting.h>
#include <service\messages\FetchManifest.h>
#include <progress\SimpleProgress.h>

struct hentsindex_tag_hentId{};
struct hentsindex_tag_hentNo{};

struct HentClientObjectKeyHentId
{
  typedef uint32_t result_type;
  result_type operator()(HentClientObject* pCO) const 
  {
	  return pCO->Object()->GetId();
  }
};

struct CowClientObjectKeyCowId
{
  typedef uint32_t result_type;
  result_type operator()(CowClientObject* pCO) const 
  {
	  return pCO->Object()->GetId();
  }
};


struct HentClientObjectKeyHentNo
{
  typedef HentNo result_type;
  result_type operator()(HentClientObject* pCO) const 
  {
	  return pCO->Object()->GetHentNo();
  }
};

enum SeasonSessionState
{
	State_Hungry,
	State_Eating,
	State_Fed
};

typedef boost::multi_index_container
<
	HentClientObject*,
	boost::multi_index::indexed_by< 
									boost::multi_index::ordered_unique < boost::multi_index::tag<hentsindex_tag_hentId>, HentClientObjectKeyHentId >,
									boost::multi_index::ordered_unique < boost::multi_index::tag<hentsindex_tag_hentNo>, HentClientObjectKeyHentNo >
								  >
> HentsIndex;

struct index_tag_Id{};

template<class CO>
struct IdableClientObjectKeyId
{
  typedef uint32_t result_type;
  result_type operator()(CO* pCO) const 
  {
	  return pCO->Object()->GetId();
  }
};



template<class CO>
struct IdableCOIndex : public boost::multi_index_container
						<
							CO*,
							boost::multi_index::indexed_by< boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_Id>, IdableClientObjectKeyId<CO> >  >
						>
{
};

typedef IdableCOIndex<InDocClientObject> InDocsIndex;
typedef IdableCOIndex<CowClientObject> CowsIndex;
typedef IdableCOIndex<BuyDocClientObject> BuyDocsIndex;
typedef IdableCOIndex<SellDocClientObject> SellDocsIndex;
typedef IdableCOIndex<MoveDocClientObject> MoveDocsIndex;
typedef IdableCOIndex<OutDocClientObject> OutDocsIndex;
typedef IdableCOIndex<BuyInvoiceClientObject> BuyInvoicesIndex;
typedef IdableCOIndex<SellInvoiceClientObject> SellInvoicesIndex;
typedef IdableIndex<Class> ClassesIndex;
typedef IdableIndex<Stock> StocksIndex;


class XContext;

class SeasonSessionImpl : public SeasonSession, protected Observerable<SeasonObserver>
{		
	class TransactionInterface : public ITransactionInterface
	{
	public:
		TransactionInterface(SeasonSessionImpl* pSession,uint32_t transactionId);
		virtual boost::any Execute(uint32_t operation,const OperationArguments& args);
		virtual boost::any Execute(uint32_t operation);
		virtual void Commit();
		virtual void Rollback();
	private:
		uint32_t m_transactionId;
		SeasonSessionImpl* m_pSession;
	};

public:
	SeasonSessionImpl();

	virtual XContext* GetContext();
	virtual ISeason* GetSeason();

	virtual void BeginFetch();
	virtual void EndFetch();
	virtual void DoFetch(ComplexProgress* progress);
	virtual void Close();

	virtual BuyDocTransactionPtr NewBuyDoc(uint32_t herdId);	
	virtual BuyDocTransactionPtr EditBuyDoc(uint32_t docId);

	virtual InDocTransactionPtr NewInDoc(uint32_t herdId);
	virtual InDocTransactionPtr EditInDoc(uint32_t docId);
	
	virtual MoveDocTransactionPtr NewMoveDoc(uint32_t fromHerdId,uint32_t toHerdId);
	virtual MoveDocTransactionPtr EditMoveDoc(uint32_t docId);	

	virtual SellDocTransactionPtr NewSellDoc(uint32_t herdId);
	virtual SellDocTransactionPtr EditSellDoc(uint32_t docId);

	virtual OutDocTransactionPtr NewOutDoc(uint32_t herdId);
	virtual OutDocTransactionPtr EditOutDoc(uint32_t docId);	

	virtual InvoiceDocsTransactionPtr NewBuyInvoiceDocs();
	virtual InvoiceDocsTransactionPtr EditBuyInvoiceDocs(uint32_t invoiceId);

	virtual InvoiceCowsTransactionPtr NewBuyInvoiceCows();
	virtual InvoiceCowsTransactionPtr EditBuyInvoiceCows(uint32_t invoiceId);

	virtual InvoiceCowsTransactionPtr NewSellInvoiceCows();
	virtual InvoiceCowsTransactionPtr EditSellInvoiceCows(uint32_t invoiceId);

	virtual InvoiceDocsTransactionPtr NewSellInvoiceDocs();
	virtual InvoiceDocsTransactionPtr EditSellInvoiceDocs(uint32_t invoiceId);
			
	virtual uint32_t InsertHent(const IHent* pHentToInsert);
	virtual void UpdateHent(const IHent* pHentToUpdate);
	virtual void DeleteHent(uint32_t hentId);
	virtual void LockHent(uint32_t hentId);
	virtual void UnlockHent(uint32_t hentId);
	virtual PtrEnumerator<HentClientObject> EnumHents();
	virtual HentClientObject* GetHent(uint32_t hentId);
	virtual HentClientObject* GetHent(const HentNo& hent);
		
	virtual void AddHerdObserver(uint32_t herdId,HerdObserver* pObserver);
	virtual void RemoveHerdObserver(uint32_t herdId,HerdObserver* pObserver);

	virtual PtrEnumerator<IStock> EnumStocks();
	virtual PtrEnumerator<IClass> EnumClasses();
	virtual PtrEnumerator<IAgent> EnumAgents();
	virtual PtrEnumerator<IBankAccount> EnumBankAccounts();

	virtual InDocClientObject* GetInDoc(uint32_t docId);
	virtual BuyDocClientObject* GetBuyDoc(uint32_t docId);
	virtual MoveDocClientObject* GetMoveDoc(uint32_t docId);			
	virtual SellDocClientObject* GetSellDoc(uint32_t docId);
	virtual OutDocClientObject* GetOutDoc(uint32_t docId);
	virtual BuyInvoiceClientObject* GetBuyInvoice(uint32_t invoiceId);
	virtual SellInvoiceClientObject* GetSellInvoice(uint32_t invoiceId);

	virtual void DeleteSellDoc(uint32_t docId);
	virtual void DeleteMoveDoc(uint32_t docId);
	virtual void DeleteOutDoc(uint32_t docId);
	virtual void DeleteBuyDoc(uint32_t docId);
	virtual void DeleteInDoc(uint32_t docId);

	virtual void LockInDoc(uint32_t docId);
	virtual void UnlockInDoc(uint32_t docId);

	virtual void LockSellDoc(uint32_t docId);
	virtual void UnlockSellDoc(uint32_t docId);

	virtual void LockMoveDoc(uint32_t docId);
	virtual void UnlockMoveDoc(uint32_t docId);

	virtual void LockOutDoc(uint32_t docId);
	virtual void UnlockOutDoc(uint32_t docId);

	virtual void LockBuyDoc(uint32_t docId);
	virtual void UnlockBuyDoc(uint32_t docId);	

	virtual void LockBuyInvoice(uint32_t invoiceId);
	virtual void UnlockBuyInvoice(uint32_t invoiceId);
	virtual void DeleteBuyInvoice(uint32_t invoiceId);

	virtual void LockSellInvoice(uint32_t invoiceId);
	virtual void UnlockSellInvoice(uint32_t invoiceId);
	virtual void DeleteSellInvoice(uint32_t invoiceId);
		
	virtual void LockCow(uint32_t cowId);
	virtual void UnlockCow(uint32_t cowId);
	virtual void UpdateCow(uint32_t cowId,const CowDetails& details);

	virtual void AddSeasonObserver(SeasonObserver* pObserver);
	virtual void RemoveSeasonObserver(SeasonObserver* pObserver);

	virtual PtrEnumerator<BuyDocClientObject> EnumBuyDocs();
	virtual PtrEnumerator<SellDocClientObject> EnumSellDocs();


	virtual PtrEnumerator<IHerd> EnumHerds();
	virtual PtrEnumerator<BuyInvoiceClientObject> EnumBuyInvoices();
	virtual PtrEnumerator<SellInvoiceClientObject> EnumSellInvoices();

	virtual IFarm* GetFarm();

	virtual UserSettings* GetUserSettings();

	virtual void OpenPurchaseRepository();
	virtual int QueryPurchases(const DateTime& startDt, const DateTime& endDt);
	virtual PurchaseHeaders FetchPurchaseHeaders();
	virtual boost::optional<Purchase> FetchPurchase(const std::wstring& purchaseId);
	virtual DownloadPurchaseTransactionPtr OpenDownloadPurchaseTransaction(const std::wstring& purchaseId);
	virtual void ClosePurchaseRepository();
	
protected:

	template<typename CO>
	CO* FindCOWithId(IdableCOIndex<CO>& COIndex,uint32_t id)
	{
		IdableCOIndex<CO>::index<index_tag_Id>::type & byIdIndex = COIndex.get<index_tag_Id>();
		auto it = byIdIndex.find(id);
		if(byIdIndex.end() != it)
			return *it;
		return NULL;
	}
	
	void FetchHents(SimpleProgress* progress);
	void FetchClasses(SimpleProgress* progress);
	void FetchStocks(SimpleProgress* progress);
	void FetchCows(SimpleProgress* progress);
	void FetchInDocs(SimpleProgress* progress);
	void FetchBuyDocs(SimpleProgress* progress);
	void FetchMoveDocs(SimpleProgress* progress);
	void FetchSellDocs(SimpleProgress* progress);
	void FetchOutDocs(SimpleProgress* progress);
	void FetchBuyInvoices(SimpleProgress* progress);
	void FetchSellInvoices(SimpleProgress* progress);
	
	CowTrail* FindCowTrail(uint32_t cowId);
	void CatalogInDocs(SimpleProgress* progress);
	void FileAnInDoc(InDoc* pDoc);

	void CatalogBuyDocs(SimpleProgress* progress);
	void FileABuyDoc(BuyDoc* pDoc);

	void FileAMoveDoc(MoveDoc* pDoc);
	void CatalogMoveDocs(SimpleProgress* progress);

	void FileASellDoc(SellDoc* pDoc);
	void CatalogSellDocs(SimpleProgress* progress);

	void FileAnOutDoc(OutDoc* pDoc);
	void CatalogOutDocs(SimpleProgress* progress);
	

	template<typename F> void VisitHerdObservers(uint32_t herdId,F f)
	{
		std::function<void (HerdObserver*)> visitor = f;

		std::function<void (std::pair<uint32_t,HerdObserver*> pair)> invokeObserver = 
			[&visitor](std::pair<uint32_t,HerdObserver*> pair)
		{
			visitor(pair.second);
		};

		auto observersRange = m_HerdObservers.equal_range(herdId);
		std::for_each(observersRange.first,observersRange.second,invokeObserver);
	}
	
	HerdRegistry* GetHerdRegistry(uint32_t herdId);
	CowClientObject* GetCow(uint32_t cowId);
	IClass* GetClass(uint32_t classId);
	IClass* GetClassByCd(const std::wstring& classCd);
	IStock* GetStock(uint32_t stockId);
	ICowTrail* GetCowTrail(uint32_t cowId); 
	IHerd* GetHerd(uint32_t herdId);
	IHerd* GetHerdByIndex(int herdIndex);
	IAgent* GetAgent(uint32_t agentId);
	IBankAccount* GetBankAccount(uint32_t accountId);
	std::vector<ICow*> GetDocCows(IDoc* pDoc);

private:
	FetchManifestObj m_fetchManifest;

	uint32_t  m_sessionToken;
	ISeason* m_season;
	RCF::TcpEndpoint  m_observerEndPoint;
	XContext* m_pContext;

	HentsIndex m_Hents;	
	CowsIndex  m_Cows;
	InDocsIndex m_InDocs;
	BuyDocsIndex m_BuyDocs;
	MoveDocsIndex m_MoveDocs;
	SellDocsIndex m_SellDocs;
	OutDocsIndex m_OutDocs;
	BuyInvoicesIndex m_BuyInvoices;
	SellInvoicesIndex m_SellInvoices;
	
	
	NotificationRelayImpl* m_pRelay;
	NotificationSubsriber* m_pNtfnSubsriber;
	std::multimap<uint32_t,HerdObserver*> m_HerdObservers;
	std::map<uint32_t,HerdRegistry*> m_Registers;	
	CowTracker* m_pTracker;

	friend class XContext;	
	friend class SeasonSessionFacadeImpl;
	friend class NotificationRelayImpl;
	friend class TransactionInterface;
	friend class SeasonSettingsImpl;

};
#endif