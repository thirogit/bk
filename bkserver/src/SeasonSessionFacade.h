#pragma once

class SeasonSession;
class ClientSession;

#include "snapshot/OwnDocSnapshot.h"
#include "snapshot/MoveDocSnapshot.h"
#include "snapshot/HentsDocSnapshot.h"
#include "snapshot/InvoiceSnapshot.h"
#include <data/IHerd.h>
#include <data/IHent.h>
#include <data/ISeason.h>
#include <data/IAgent.h>
#include <data/IInDoc.h>
#include <data/IBuyDoc.h>
#include <data/IMoveDoc.h>
#include <data/ISellDoc.h>
#include <data/IOutDoc.h>
#include <data/IBuyInvoice.h>
#include <data/ISellInvoice.h>
#include <logic/cowtrail/ICowTrail.h>
#include <logic/herdregistry/ICowPresence.h>
#include <data/repo/IPurchase.h>
#include <logic/herdregistry/CowPresenceKeys.h>

class SeasonSessionFacade
{
public:
	virtual void LockHent(uint32_t hentId) = 0;
	virtual void UnlockHent(uint32_t hentId) = 0;
	virtual uint32_t InsertHent(IHent* hent) = 0;
	virtual void UpdateHent(IHent* hent) = 0;
	virtual void DeleteHent(uint32_t hentId) = 0;
	virtual IHent* FindHent(uint32_t hentId) = 0;
	virtual IHent* FindHent(const HentNo& hentNo) = 0;

	virtual void LockCow(uint32_t cowId) = 0;
	virtual void UnlockCow(uint32_t cowId) = 0;
	virtual void UpdateCow(uint32_t cowId,const CowDetails& details) = 0;
	virtual ICow* FindCow(uint32_t cowId) = 0;

	virtual bool IsInDocLocked(uint32_t docId) = 0;
	virtual IInDoc* FindInDoc(uint32_t docId) = 0;
	virtual void LockInDoc(uint32_t docId) = 0;
	virtual void LockInDocWithCows(uint32_t docId) = 0;

	virtual void UnlockInDoc(uint32_t docId) = 0;
	virtual void UpdateInDoc(const OwnDocSnapshot& shapshot) = 0;
	virtual void InsertInDoc(const OwnDocSnapshot& shapshot) = 0;
	virtual void DeleteInDoc(uint32_t docId) = 0;

	virtual bool IsBuyDocLocked(uint32_t docId) = 0;
	virtual IBuyDoc* FindBuyDoc(uint32_t docId) = 0;
	virtual void LockBuyDoc(uint32_t docId) = 0;
	virtual void UnlockBuyDoc(uint32_t docId) = 0;
	virtual void UpdateBuyDoc(const HentsDocSnapshot& shapshot) = 0;
	virtual void InsertBuyDoc(const HentsDocSnapshot& shapshot) = 0;
	virtual void DeleteBuyDoc(uint32_t docId) = 0;

	virtual bool IsMoveDocLocked(uint32_t docId) = 0;
	virtual IMoveDoc* FindMoveDoc(uint32_t docId) = 0;
	virtual void LockMoveDoc(uint32_t docId) = 0;
	virtual void UnlockMoveDoc(uint32_t docId) = 0;
	virtual void UpdateMoveDoc(const MoveDocSnapshot& shapshot) = 0;
	virtual void InsertMoveDoc(const MoveDocSnapshot& shapshot) = 0;
	virtual void DeleteMoveDoc(uint32_t docId) = 0;
	
	virtual bool IsSellDocLocked(uint32_t docId) = 0;
	virtual ISellDoc* FindSellDoc(uint32_t docId) = 0;
	virtual void LockSellDoc(uint32_t docId) = 0;
	virtual void UnlockSellDoc(uint32_t docId) = 0;
	virtual void UpdateSellDoc(const HentsDocSnapshot& shapshot) = 0;
	virtual void InsertSellDoc(const HentsDocSnapshot& shapshot) = 0;
	virtual void DeleteSellDoc(uint32_t docId) = 0;

	virtual bool IsOutDocLocked(uint32_t docId) = 0;
	virtual IOutDoc* FindOutDoc(uint32_t docId) = 0;
	virtual void LockOutDoc(uint32_t docId) = 0;
	virtual void UnlockOutDoc(uint32_t docId) = 0;
	virtual void UpdateOutDoc(const OwnDocSnapshot& shapshot) = 0;
	virtual void InsertOutDoc(const OwnDocSnapshot& shapshot) = 0;
	virtual void DeleteOutDoc(uint32_t docId) = 0;

	virtual IBuyInvoice* FindBuyInvoice(uint32_t invoiceId) = 0;
	virtual bool IsBuyInvoiceLocked(uint32_t invoiceId) = 0;
	virtual void LockBuyInvoice(uint32_t invoiceId) = 0;
	virtual void UnlockBuyInvoice(uint32_t invoiceId) = 0;

	virtual void InsertBuyDocsInvoice(const InvoiceSnapshot& snapshot) = 0;
	virtual void UpdateBuyDocsInvoice(const InvoiceSnapshot& snapshot) = 0;
	virtual void InsertBuyCowsInvoice(const InvoiceSnapshot& snapshot) = 0;
	virtual void UpdateBuyCowsInvoice(const InvoiceSnapshot& snapshot) = 0;

	virtual void DeleteBuyInvoice(uint32_t invoiceId) = 0;
	
	virtual ISellInvoice* FindSellInvoice(uint32_t invoiceId) = 0;
	virtual bool IsSellInvoiceLocked(uint32_t invoiceId) = 0;
	virtual void LockSellInvoice(uint32_t invoiceId) = 0;
	virtual void UnlockSellInvoice(uint32_t invoiceId) = 0;

	virtual void InsertSellDocsInvoice(const InvoiceSnapshot& snapshot) = 0;
	virtual void UpdateSellDocsInvoice(const InvoiceSnapshot& snapshot) = 0;
	virtual void InsertSellCowsInvoice(const InvoiceSnapshot& snapshot) = 0;
	virtual void UpdateSellCowsInvoice(const InvoiceSnapshot& snapshot) = 0;

	virtual void DeleteSellInvoice(uint32_t invoiceId) = 0;

	virtual void InsertPurchase(IPurchase* purchase, const DateTime& docDt) = 0;

	virtual IAgent* FindAgent(uint32_t agentId) = 0;
	virtual const ISeason* GetSeason() = 0;
	virtual ICowTrail* GetCowTrail(uint32_t cowId) = 0;
	virtual ICowPresence* GetCowPresence(uint32_t herdId,const CowPresenceKey& key) = 0;
	virtual CowPresenceKeys GetCowPresences(uint32_t herdId, const CowNo& cowNo) = 0;

	virtual IClass* FindClass(const std::wstring& classCd) = 0;
	virtual IStock* FindStock(const std::wstring& stockCd) = 0;
	virtual IHerd* FindHerdByIndex(int herdIndex) = 0;
	virtual IHerd* FindHerdById(uint32_t herdId) = 0;
	virtual IAgent* FindAgent(const std::wstring&  agentCd) = 0;

	virtual bool IsCowInStock(uint32_t herdId, const CowNo& cowNo) = 0;
};

