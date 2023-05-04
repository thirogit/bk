#pragma once

#include "../../data/IHent.h"
#include "../../data/ICow.h"
#include "../../data/IInDoc.h"
#include "../../data/IMoveDoc.h"
#include "../../data/IBuyDoc.h"
#include "../../data/ISellDoc.h"
#include "../../data/IOutDoc.h"
#include "../../data/IBuyInvoice.h"
#include "../../data/ISellInvoice.h"

class NotificationRelay
{
public:
	virtual void HentUnlock(uint32_t hentId) = 0;
	virtual void HentLock(uint32_t hentId) = 0;	
	virtual void HentInserted(IHent* pHent) = 0;
	virtual void HentUpdated(IHent* pHent) = 0;
	virtual void HentDeleted(uint32_t hentId) = 0;

	virtual void CowLock(uint32_t cowId) = 0;
	virtual void CowUnlock(uint32_t cowId) = 0;
	virtual void CowInserted(ICow* pCow) = 0;
	virtual void CowUpdated(ICow* pCow) = 0;
	virtual void CowDeleted(uint32_t cowId) = 0;

	virtual void InDocLock(uint32_t docId) = 0;
	virtual void InDocUnlock(uint32_t docId) = 0;
	virtual void InDocInserted(IInDoc* pDoc) = 0;
	virtual void InDocUpdated(IInDoc* pDoc) = 0;
	virtual void InDocDeleted(uint32_t docId) = 0;

	virtual void BuyDocLock(uint32_t docId) = 0;
	virtual void BuyDocUnlock(uint32_t docId) = 0;
	virtual void BuyDocInserted(IBuyDoc* pDoc) = 0;
	virtual void BuyDocUpdated(IBuyDoc* pDoc) = 0;
	virtual void BuyDocDeleted(uint32_t docId) = 0;

	virtual void MoveDocLock(uint32_t docId) = 0;
	virtual void MoveDocUnlock(uint32_t docId) = 0;
	virtual void MoveDocInserted(IMoveDoc* pDoc) = 0;
	virtual void MoveDocUpdated(IMoveDoc* pDoc) = 0;
	virtual void MoveDocDeleted(uint32_t docId) = 0;

	virtual void SellDocLock(uint32_t docId) = 0;
	virtual void SellDocUnlock(uint32_t docId) = 0;
	virtual void SellDocInserted(ISellDoc* pDoc) = 0;
	virtual void SellDocUpdated(ISellDoc* pDoc) = 0;
	virtual void SellDocDeleted(uint32_t docId) = 0;

	virtual void OutDocLock(uint32_t docId) = 0;
	virtual void OutDocUnlock(uint32_t docId) = 0;
	virtual void OutDocInserted(IOutDoc* pDoc) = 0;
	virtual void OutDocUpdated(IOutDoc* pDoc) = 0;
	virtual void OutDocDeleted(uint32_t docId) = 0;

	virtual void BuyInvoiceLock(uint32_t invoiceId) = 0;
	virtual void BuyInvoiceInserted(IBuyInvoice* pInvoice) = 0;
	virtual void BuyInvoiceUpdated(IBuyInvoice* pInvoice) = 0;
	virtual void BuyInvoiceDeleted(uint32_t invoiceId) = 0;
	virtual void BuyInvoiceUnlock(uint32_t invoiceId) = 0;

	virtual void SellInvoiceLock(uint32_t invoiceId) = 0;
	virtual void SellInvoiceInserted(ISellInvoice* pInvoice) = 0;
	virtual void SellInvoiceUpdated(ISellInvoice* pInvoice) = 0;
	virtual void SellInvoiceDeleted(uint32_t invoiceId) = 0;
	virtual void SellInvoiceUnlock(uint32_t invoiceId) = 0;
	
	virtual void BeginFetch() = 0;
	virtual void EndFetch() = 0;
};

