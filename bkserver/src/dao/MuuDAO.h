#ifndef __MUUDAO_H__
#define __MUUDAO_H__

#include <progress/SimpleProgress.h>
#include <progress/ComplexProgress.h>
#include "../db/Transaction.h"
#include "BaseDAO.h"
#include <arrays/NewPtrFlushArray.h>
#include <data/impl/Farm.h>
#include <data/impl/Herd.h>
#include <data/impl/Season.h>
#include <data/impl/User.h>
#include <data/impl/Hent.h>
#include <data/impl/Class.h>
#include <data/impl/Stock.h>
#include <data/impl/Cow.h>
#include <data/impl/InDoc.h>
#include <data/impl/BuyDoc.h>
#include <data/impl/SellDoc.h>
#include <data/impl/MoveDoc.h>
#include <data/impl/OutDoc.h>
#include <data/impl/BuyInvoice.h>
#include <data/impl/SellInvoice.h>
#include <data/impl/Agent.h>
#include <data/impl/BankAccount.h>


class MuuDAO : public BaseDAO
{
	typedef std::multimap<uint32_t,CowEntry*> EntriesMap;
	typedef std::multimap<uint32_t,CowInvoiceEntry*> InvoiceEntriesMap;
	typedef std::map<uint32_t,InvoiceHent*> InvoiceHentsMap;
	typedef std::map<uint32_t, InvoiceDeduction*> InvoiceDeductionsMap;
		
public:
	Farm* LoadFarm(const FarmNo& farmNo);
	
	bool LoadHerds(NewPtrFlushArray<Herd>& result,IFarm* pFarm,SimpleProgress *progress = NULL);
	void UpdateHerd(Transaction transaction,const IHerd* pHerd);
	void DeleteHerd(Transaction transaction,const IHerd* pHerd);
	Herd* InsertHerd(Transaction transaction,const IHerd* pHerd);

	bool LoadSeasons(NewPtrFlushArray<Season>& result,IFarm* pFarm,SimpleProgress *progress = NULL);
	void UpdateSeason(Transaction transaction,const ISeason* pSeason);
	void DeleteSeason(Transaction transaction,const ISeason* pSeason);
	Season* InsertSeason(Transaction transaction,const ISeason* pSeason);

	bool LoadUsers(NewPtrFlushArray<User>& result,IFarm* pFarm,SimpleProgress *progress = NULL);
	void UpdateUser(Transaction transaction,const IUser* pUser);
	void DeleteUser(Transaction transaction,const IUser* pSeason);
	User* InsertUser(Transaction transaction,const IUser* pSeason);
	
	bool LoadHents(NewPtrFlushArray<Hent>& result,uint32_t seasonId,SimpleProgress *progress = NULL);
	bool UpdateHent(Transaction transaction,IHent* pHent);
	bool DeleteHent(Transaction transaction,IHent* pHent);
	Hent* InsertHent(Transaction transaction,uint32_t season,IHent* pHent);

	bool LoadClasses(NewPtrFlushArray<Class>& result,uint32_t farmId,SimpleProgress *progress = NULL);
	bool LoadStocks(NewPtrFlushArray<Stock>& result,uint32_t farmId,SimpleProgress *progress = NULL);
	bool LoadCows(NewPtrFlushArray<Cow>& result,uint32_t seasonId,SimpleProgress *progress = NULL);
	Cow* InsertCow(Transaction transaction,uint32_t season,const CowDetails& cowDetails);
	bool DeleteCow(Transaction transaction,uint32_t cowId);
	void UpdateCow(Transaction transaction,uint32_t cowId,const CowDetails& cowDetails);
	void UpdateCow(Transaction transaction,ICow* pCow);
	void UpdateCowWithBuyInvoice(Transaction transaction,uint32_t cowId,uint32_t invoiceId);
	void UpdateCowWithSellInvoice(Transaction transaction,uint32_t cowId,uint32_t invoiceId);
	
	bool LoadInDocs(NewPtrFlushArray<InDoc>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	InDoc* InsertInDoc(Transaction transaction,uint32_t season,IInDoc* pDoc);
	bool DeleteInDoc(Transaction transaction,uint32_t docId);	
	void UpdateInDoc(Transaction transaction,const IInDoc* pDoc);

	bool LoadBuyDocs(NewPtrFlushArray<BuyDoc>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	BuyDoc* InsertBuyDoc(Transaction transaction,uint32_t season,IBuyDoc* pDoc);
	bool DeleteBuyDoc(Transaction transaction,uint32_t docId);
	void UpdateBuyDoc(Transaction transaction,const IBuyDoc* pDoc);
	void UpdateBuyDocWithInvoice(Transaction transaction,uint32_t docId,uint32_t invoiceId);
	
	bool LoadMoveDocs(NewPtrFlushArray<MoveDoc>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	MoveDoc* InsertMoveDoc(Transaction transaction,uint32_t season,IMoveDoc* pDoc);
	bool DeleteMoveDoc(Transaction transaction,uint32_t docId);	
	void UpdateMoveDoc(Transaction transaction,const IMoveDoc* pDoc);

	bool LoadSellDocs(NewPtrFlushArray<SellDoc>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	SellDoc* InsertSellDoc(Transaction transaction,uint32_t season,ISellDoc* pDoc);
	bool DeleteSellDoc(Transaction transaction,uint32_t docId);
	void UpdateSellDoc(Transaction transaction,ISellDoc* pDoc);
	void UpdateSellDocWithInvoice(Transaction transaction,uint32_t docId,uint32_t invoiceId);
	
	bool LoadOutDocs(NewPtrFlushArray<OutDoc>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	OutDoc* InsertOutDoc(Transaction transaction,uint32_t season,IOutDoc* pDoc);
	bool DeleteOutDoc(Transaction transaction,uint32_t docId);
	void UpdateOutDoc(Transaction transaction,IOutDoc* pDoc);
	void InsertOutDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,ICowEntry* pEntry);

	bool LoadBuyInvoices(NewPtrFlushArray<BuyInvoice>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	BuyInvoice* InsertBuyInvoice(Transaction transaction,uint32_t season,IBuyInvoice* pInvoice);
	bool DeleteBuyInvoice(Transaction transaction,uint32_t invoiceId);
	void UpdateBuyInvoice(Transaction transaction,IBuyInvoice* pInvoice);
	
	bool LoadSellInvoices(NewPtrFlushArray<SellInvoice>& result,uint32_t seasonId,ComplexProgress *progress = NULL);
	SellInvoice* InsertSellInvoice(Transaction transaction,uint32_t season,ISellInvoice* pInvoice);
	bool DeleteSellInvoice(Transaction transaction,uint32_t invoiceId);
	void UpdateSellInvoice(Transaction transaction,ISellInvoice* pInvoice);	

	bool LoadAgents(NewPtrFlushArray<Agent>& result,uint32_t farmId,SimpleProgress *progress = NULL);
	bool UpdateAgent(Transaction transaction,IAgent* pHent);
	bool DeleteAgent(Transaction transaction,IAgent* pHent);
	Agent* InsertAgent(Transaction transaction,uint32_t season,IAgent* pAgent);

	void EnsureCounter(Transaction transaction, const std::wstring& sCounterName, uint32_t season);
	uint32_t NextCounterValue(Transaction transaction,const std::wstring& sCounterName,uint32_t season);
	void SetCounterValue(Transaction transaction,uint32_t value,const std::wstring& sCounterName,uint32_t season);
	uint32_t GetCounterValue(Transaction transaction, const std::wstring& sCounterName, uint32_t season);
	void CreateCounter(Transaction transaction,const std::wstring& sCounterName,uint32_t season);

	bool LoadBankAccounts(NewPtrFlushArray<BankAccount>& result, uint32_t farmId, SimpleProgress *progress = NULL);

private:
	void InsertInvoice(Transaction transaction,const std::wstring& sTable,uint32_t season,uint32_t invoiceId,IInvoice* pInvoice);
	void InsertInvoiceHent(Transaction transaction,const std::wstring& sTable,const IInvoiceHent* pHent,uint32_t invoiceId,uint32_t season);
	void InsertInvoiceEntry(Transaction transaction,const std::wstring& sTable,const ICowInvoiceEntry* pEntry,uint32_t invoiceId,uint32_t season);
	void InsertInvoiceDeduction(Transaction transaction, const std::wstring& sTable, const IInvoiceDeduction* pDeduction, uint32_t invoiceId, uint32_t season);

	void InsertBuyInvoiceHent(Transaction transaction,const IInvoiceHent* pHent,uint32_t invoiceId,uint32_t season);
	void InsertBuyInvoiceEntry(Transaction transaction,const ICowInvoiceEntry* pEntry,uint32_t invoiceId,uint32_t season);
	void InsertBuyInvoiceDeduction(Transaction transaction, const IInvoiceDeduction* pDeduction, uint32_t invoiceId, uint32_t season);

	void InsertSellInvoiceHent(Transaction transaction,const IInvoiceHent* pHent,uint32_t invoiceId,uint32_t season);
	void InsertSellInvoiceEntry(Transaction transaction,const ICowInvoiceEntry* pEntry,uint32_t invoiceId,uint32_t season);
	void InsertSellInvoiceDeduction(Transaction transaction, const IInvoiceDeduction* pDeduction, uint32_t invoiceId, uint32_t season);
	
	void InsertMoveDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry);
	void InsertBuyDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry);
	void InsertInDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry);
	void InsertSellDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry);	
	void InsertCowEntry(Transaction transaction,const std::wstring& sTable,uint32_t season,uint32_t docId,const ICowEntry* pEntry);
	bool LoadInDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadBuyDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadMoveDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadSellDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadOutDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadCowsFromTable(const std::wstring& sTableName,EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool ExtractCowEntries(Cursor& cursor, EntriesMap& result, SimpleProgress *pProgress = NULL);
	bool DeleteCowEntries(Transaction transaction,const std::wstring& sTable,uint32_t docId);
	bool DeleteInDocCowEntries(Transaction transaction,uint32_t docId);
	bool DeleteBuyDocCowEntries(Transaction transaction,uint32_t docId);
	bool DeleteMoveDocCowEntries(Transaction transaction,uint32_t docId);
	bool DeleteSellDocCowEntries(Transaction transaction,uint32_t docId);
	bool DeleteOutDocCowEntries(Transaction transaction,uint32_t docId);	

	bool LoadBuyInvoicesEntries(InvoiceEntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadSellInvoicesEntries(InvoiceEntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);

	bool LoadBuyInvoiceHents(InvoiceHentsMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadBuyInvoiceDeductions(InvoiceDeductionsMap& result, uint32_t seasonId, SimpleProgress *pProgress = NULL);
	bool LoadSellInvoiceHents(InvoiceHentsMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadSellInvoiceDeductions(InvoiceDeductionsMap& result, uint32_t seasonId, SimpleProgress *pProgress = NULL);

	bool LoadInvoiceHentsFromTable(const std::wstring& sTableName,InvoiceHentsMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadInvoicesEntriesFromTable(const std::wstring& sTableName,InvoiceEntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress = NULL);
	bool LoadInvoicesDeductionsFromTable(const std::wstring& sTableName, InvoiceDeductionsMap& result, uint32_t seasonId, SimpleProgress *pProgress = NULL);
	bool ExtractInvoiceEntries(Cursor& cursor, InvoiceEntriesMap& result, SimpleProgress *pProgress = NULL);
	bool ExtractInvoiceHents(Cursor& cursor, InvoiceHentsMap& result, SimpleProgress *pProgress = NULL);
	bool ExtractInvoiceDeductions(Cursor& cursor, InvoiceDeductionsMap& result, SimpleProgress *pProgress = NULL);
		
	bool DeleteBuyInvoiceCowEntries(Transaction transaction,uint32_t invoiceId);
	bool DeleteBuyInvoiceHent(Transaction transaction,uint32_t invoiceId);
	bool DeleteBuyInvoiceDeductions(Transaction transaction, uint32_t invoiceId);

	bool DeleteSellInvoiceCowEntries(Transaction transaction,uint32_t invoiceId);
	bool DeleteSellInvoiceHent(Transaction transaction,uint32_t invoiceId);
	bool DeleteSellInvoiceDeductions(Transaction transaction, uint32_t invoiceId);

	bool DeleteInvoiceCowEntries(Transaction transaction,const std::wstring& sTable,uint32_t invoiceId);
	bool DeleteInvoiceHent(Transaction transaction,const std::wstring& sTable,uint32_t invoiceId);
	bool DeleteInvoiceDeductions(Transaction transaction, const std::wstring& sTable, uint32_t invoiceId);
};

#endif
