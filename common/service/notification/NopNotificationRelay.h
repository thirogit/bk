#pragma once

#include "NotificationRelay.h"

	class NopNotificationRelay : public NotificationRelay
	{
	public:
		NopNotificationRelay();

		virtual void HentUnlock(uint32_t hentId);
		virtual void HentLock(uint32_t hentId);
		virtual void HentInserted(IHent* pHent);
		virtual void HentUpdated(IHent* pHent);
		virtual void HentDeleted(uint32_t hentId);

		virtual void CowLock(uint32_t cowId);
		virtual void CowUnlock(uint32_t cowId);
		virtual void CowInserted(ICow* pCow);
		virtual void CowUpdated(ICow* pCow);
		virtual void CowDeleted(uint32_t cowId);

		virtual void InDocLock(uint32_t docId);
		virtual void InDocUnlock(uint32_t docId);
		virtual void InDocInserted(IInDoc* pDoc);
		virtual void InDocUpdated(IInDoc* pDoc);
		virtual void InDocDeleted(uint32_t docId);

		virtual void BuyDocLock(uint32_t docId);
		virtual void BuyDocUnlock(uint32_t docId);
		virtual void BuyDocInserted(IBuyDoc* pDoc);
		virtual void BuyDocUpdated(IBuyDoc* pDoc);
		virtual void BuyDocDeleted(uint32_t docId);

		virtual void MoveDocLock(uint32_t docId);
		virtual void MoveDocUnlock(uint32_t docId);
		virtual void MoveDocInserted(IMoveDoc* pDoc);
		virtual void MoveDocUpdated(IMoveDoc* pDoc);
		virtual void MoveDocDeleted(uint32_t docId);

		virtual void SellDocLock(uint32_t docId);
		virtual void SellDocUnlock(uint32_t docId);
		virtual void SellDocInserted(ISellDoc* pDoc);
		virtual void SellDocUpdated(ISellDoc* pDoc);
		virtual void SellDocDeleted(uint32_t docId);

		virtual void OutDocLock(uint32_t docId);
		virtual void OutDocUnlock(uint32_t docId);
		virtual void OutDocInserted(IOutDoc* pDoc);
		virtual void OutDocUpdated(IOutDoc* pDoc);
		virtual void OutDocDeleted(uint32_t docId);

		virtual void BuyInvoiceLock(uint32_t invoiceId);
		virtual void BuyInvoiceInserted(IBuyInvoice* pInvoice);
		virtual void BuyInvoiceUpdated(IBuyInvoice* pInvoice);
		virtual void BuyInvoiceDeleted(uint32_t invoiceId);
		virtual void BuyInvoiceUnlock(uint32_t invoiceId);

		virtual void SellInvoiceLock(uint32_t invoiceId);
		virtual void SellInvoiceInserted(ISellInvoice* pInvoice);
		virtual void SellInvoiceUpdated(ISellInvoice* pInvoice);
		virtual void SellInvoiceDeleted(uint32_t invoiceId);
		virtual void SellInvoiceUnlock(uint32_t invoiceId);

		virtual void BeginFetch();
		virtual void EndFetch();

};

