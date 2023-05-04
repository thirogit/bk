#pragma once

#include "../SeasonSessionFacade.h"
#include "../SeasonSession.h"
#include "../ClientSeasonSession.h"
#include "../index/HentsIndex.h"

class SeasonSessionFacadeImpl : public SeasonSessionFacade
{
public:
	SeasonSessionFacadeImpl(ClientSeasonSession* pClient,SeasonSession* pDelegate);

	virtual void LockHent(uint32_t hentId);
	virtual void UnlockHent(uint32_t hentId);
	virtual uint32_t InsertHent(IHent* hent);
	virtual void UpdateHent(IHent* hent);
	virtual void DeleteHent(uint32_t hentId);
	virtual IHent* FindHent(uint32_t hentId);
	virtual IHent* FindHent(const HentNo& hentNo);

	virtual void LockCow(uint32_t cowId);
	virtual void UnlockCow(uint32_t cowId);
	virtual void UpdateCow(uint32_t cowId,const CowDetails& details);

	virtual bool IsInDocLocked(uint32_t docId);
	virtual IInDoc* FindInDoc(uint32_t docId);
	virtual void LockInDoc(uint32_t docId);
	virtual void LockInDocWithCows(uint32_t docId);
	virtual void UnlockInDoc(uint32_t docId);
	virtual void UpdateInDoc(const OwnDocSnapshot& shapshot);
	virtual void InsertInDoc(const OwnDocSnapshot& shapshot);
	virtual void DeleteInDoc(uint32_t docId);

	
	virtual bool IsBuyDocLocked(uint32_t docId);
	virtual IBuyDoc* FindBuyDoc(uint32_t docId);
	virtual void LockBuyDoc(uint32_t docId);
	virtual void UnlockBuyDoc(uint32_t docId);
	virtual void UpdateBuyDoc(const HentsDocSnapshot& shapshot);
	virtual void InsertBuyDoc(const HentsDocSnapshot& shapshot);
	virtual void DeleteBuyDoc(uint32_t docId);

	virtual bool IsMoveDocLocked(uint32_t docId);		
	virtual IMoveDoc* FindMoveDoc(uint32_t docId);
	virtual void LockMoveDoc(uint32_t docId);
	virtual void UnlockMoveDoc(uint32_t docId);
	virtual void UpdateMoveDoc(const MoveDocSnapshot& shapshot);
	virtual void InsertMoveDoc(const MoveDocSnapshot& shapshot);
	virtual void DeleteMoveDoc(uint32_t docId);

	virtual bool IsSellDocLocked(uint32_t docId);
	virtual ISellDoc* FindSellDoc(uint32_t docId);
	virtual void LockSellDoc(uint32_t docId);
	virtual void UnlockSellDoc(uint32_t docId);
	virtual void UpdateSellDoc(const HentsDocSnapshot& shapshot);
	virtual void InsertSellDoc(const HentsDocSnapshot& shapshot);
	virtual void DeleteSellDoc(uint32_t docId);

	virtual bool IsOutDocLocked(uint32_t docId);
	virtual IOutDoc* FindOutDoc(uint32_t docId);
	virtual void LockOutDoc(uint32_t docId);
	virtual void UnlockOutDoc(uint32_t docId);
	virtual void UpdateOutDoc(const OwnDocSnapshot& shapshot);
	virtual void InsertOutDoc(const OwnDocSnapshot& shapshot);
	virtual void DeleteOutDoc(uint32_t docId);
	
	virtual IAgent* FindAgent(uint32_t agentId);
	virtual ISeason* GetSeason();
	virtual ICow* FindCow(uint32_t cowId);
	virtual ICowTrail* GetCowTrail(uint32_t cowId);
	virtual ICowPresence* GetCowPresence(uint32_t herdId,const CowPresenceKey& key);
	virtual CowPresenceKeys GetCowPresences(uint32_t herdId, const CowNo& cowNo);

	virtual IBuyInvoice* FindBuyInvoice(uint32_t invoiceId);
	virtual bool IsBuyInvoiceLocked(uint32_t invoiceId);
	virtual void LockBuyInvoice(uint32_t invoiceId);
	virtual void UnlockBuyInvoice(uint32_t invoiceId);

	virtual void InsertBuyDocsInvoice(const InvoiceSnapshot& snapshot);
	virtual void UpdateBuyDocsInvoice(const InvoiceSnapshot& snapshot);
	virtual void InsertBuyCowsInvoice(const InvoiceSnapshot& snapshot);
	virtual void UpdateBuyCowsInvoice(const InvoiceSnapshot& snapshot);

	virtual void DeleteBuyInvoice(uint32_t invoiceId);

	virtual ISellInvoice* FindSellInvoice(uint32_t invoiceId);
	virtual bool IsSellInvoiceLocked(uint32_t invoiceId);
	virtual void LockSellInvoice(uint32_t invoiceId);
	virtual void UnlockSellInvoice(uint32_t invoiceId);

	virtual void InsertSellDocsInvoice(const InvoiceSnapshot& snapshot);
	virtual void UpdateSellDocsInvoice(const InvoiceSnapshot& snapshot);
	virtual void InsertSellCowsInvoice(const InvoiceSnapshot& snapshot);
	virtual void UpdateSellCowsInvoice(const InvoiceSnapshot& snapshot);

	virtual void DeleteSellInvoice(uint32_t invoiceId);

	template<class O> O* SafeFind(uint32_t id,ServerObject<O>* (SeasonSession::*findFn)(uint32_t))
	{
		ServerObject<O>* pSO = (m_pDelegate->*findFn)(id);
		if(pSO)
			return pSO->Object();
		return NULL;
	}

	virtual IClass* FindClass(const std::wstring& classCd);
	virtual IStock* FindStock(const std::wstring& stockCd);
	virtual IHerd* FindHerdByIndex(int herdIndex);
	virtual IHerd* FindHerdById(uint32_t herdId);
	virtual IAgent* FindAgent(const std::wstring&  agentCd);

	void ValidateMandatoryForHent(IHent* hent);
	void ThrowHentNotFound(uint32_t hentId);

	virtual void InsertPurchase(IPurchase* purchase,const DateTime& docDt);
	virtual bool IsCowInStock(uint32_t herdId, const CowNo& cowNo);

private:	
	ClientSeasonSession* m_pClient;
	SeasonSession* m_pDelegate;	

};

