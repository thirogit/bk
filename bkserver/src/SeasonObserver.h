#pragma once

#include <data/IHent.h>
#include <data/ICow.h>
#include <data/IInDoc.h>
#include <data/IBuyDoc.h>
#include <data/IMoveDoc.h>
#include <data/ISellDoc.h>
#include <data/IOutDoc.h>
#include <data/IBuyInvoice.h>
#include <data/ISellInvoice.h>

class SeasonObserver
{
public:
	virtual void OnHentLock(uint32_t hentId) = 0;
	virtual void OnHentUnlock(uint32_t hentId) = 0;
	virtual void OnHentInserted(IHent* pHent) = 0;
	virtual void OnHentUpdated(IHent* pHent) = 0;
	virtual void OnHentDeleted(uint32_t hentId) = 0;
	virtual void OnBeginFetch() = 0;

	virtual void OnCowLock(uint32_t cowId) = 0;
	virtual void OnCowUnlock(uint32_t cowId) = 0;
	virtual void OnCowInserted(ICow* pCow) = 0;
	virtual void OnCowUpdated(ICow* pCow) = 0;
	virtual void OnCowDeleted(uint32_t cowId) = 0;

	virtual void OnInDocLock(uint32_t docId) = 0;
	virtual void OnInDocUnlock(uint32_t docId) = 0;
	virtual void OnInDocInserted(IInDoc* pDoc) = 0;
	virtual void OnInDocDeleted(uint32_t docId) = 0;
	virtual void OnInDocUpdated(IInDoc* pDoc) = 0;

	virtual void OnBuyDocLock(uint32_t docId) = 0;
	virtual void OnBuyDocUnlock(uint32_t docId) = 0;
	virtual void OnBuyDocInserted(IBuyDoc* pDoc) = 0;
	virtual void OnBuyDocDeleted(uint32_t docId) = 0;
	virtual void OnBuyDocUpdated(IBuyDoc* pDoc) = 0;	

	virtual void OnMoveDocLock(uint32_t docId) = 0;
	virtual void OnMoveDocUnlock(uint32_t docId) = 0;
	virtual void OnMoveDocInserted(IMoveDoc* pDoc) = 0;
	virtual void OnMoveDocDeleted(uint32_t docId) = 0;
	virtual void OnMoveDocUpdated(IMoveDoc* pDoc) = 0;

	virtual void OnSellDocLock(uint32_t docId) = 0;
	virtual void OnSellDocUnlock(uint32_t docId) = 0;
	virtual void OnSellDocInserted(ISellDoc* pDoc) = 0;
	virtual void OnSellDocDeleted(uint32_t docId) = 0;
	virtual void OnSellDocUpdated(ISellDoc* pDoc) = 0;

	virtual void OnOutDocLock(uint32_t docId) = 0;
	virtual void OnOutDocUnlock(uint32_t docId) = 0;
	virtual void OnOutDocInserted(IOutDoc* pDoc) = 0;
	virtual void OnOutDocDeleted(uint32_t docId) = 0;
	virtual void OnOutDocUpdated(IOutDoc* pDoc) = 0;

	virtual void OnBuyInvoiceLocked(uint32_t invoiceId) = 0;
	virtual void OnBuyInvoiceInserted(IBuyInvoice* pInvoice) = 0;
	virtual void OnBuyInvoiceUpdated(IBuyInvoice* pInvoice) = 0;
	virtual void OnBuyInvoiceDelete(uint32_t invoiceId) = 0;
	virtual void OnBuyInvoiceUnlocked(uint32_t invoiceId) = 0;

	virtual void OnSellInvoiceLocked(uint32_t invoiceId) = 0;
	virtual void OnSellInvoiceInserted(ISellInvoice* pInvoice) = 0;
	virtual void OnSellInvoiceUpdated(ISellInvoice* pInvoice) = 0;
	virtual void OnSellInvoiceDelete(uint32_t invoiceId) = 0;
	virtual void OnSellInvoiceUnlocked(uint32_t invoiceId) = 0;
	
};