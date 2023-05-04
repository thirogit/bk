#ifndef __SEASONOBSERVER_H__
#define __SEASONOBSERVER_H__

#include <stdint.h>

class SeasonObserver
{
public:
	virtual void  OnHentLocked(uint32_t hentId) = 0;
	virtual void  OnHentInserted(uint32_t hentId) = 0;
	virtual void  OnHentUpdated(uint32_t hentId) = 0;
	virtual void  OnHentDeleted(uint32_t hentId) = 0;
	virtual void  OnHentUnlocked(uint32_t hentId) = 0;
	
	virtual void  OnCowLocked(uint32_t cowId) = 0;
	virtual void  OnCowInserted(uint32_t cowId) = 0;
	virtual void  OnCowUpdated(uint32_t cowId) = 0;
	virtual void  OnCowDeleted(uint32_t cowId) = 0;
	virtual void  OnCowUnlocked(uint32_t cowId) = 0;

	virtual void OnInDocInserted(uint32_t docId) = 0;
	virtual void OnInDocUpdated(uint32_t docId) = 0;
	virtual void OnInDocDeleted(uint32_t docId) = 0;
	virtual void OnInDocLocked(uint32_t docId) = 0;
	virtual void OnInDocUnlocked(uint32_t docId) = 0;

	virtual void OnBuyDocInserted(uint32_t docId) = 0;
	virtual void OnBuyDocUpdated(uint32_t docId) = 0;
	virtual void OnBuyDocDeleted(uint32_t docId) = 0;
	virtual void OnBuyDocLocked(uint32_t docId) = 0;
	virtual void OnBuyDocUnlocked(uint32_t docId) = 0;

	virtual void OnMoveDocLocked(uint32_t docId) = 0;
	virtual void OnMoveDocUnlocked(uint32_t docId) = 0;
	virtual void OnMoveDocInserted(uint32_t docId) = 0;
	virtual void OnMoveDocUpdated(uint32_t docId) = 0;
	virtual void OnMoveDocDeleted(uint32_t docId) = 0;

	virtual void OnSellDocInserted(uint32_t docId) = 0;
	virtual void OnSellDocUpdated(uint32_t docId) = 0;
	virtual void OnSellDocDeleted(uint32_t docId) = 0;
	virtual void OnSellDocLocked(uint32_t docId) = 0;
	virtual void OnSellDocUnlocked(uint32_t docId) = 0;	

	virtual void OnOutDocInserted(uint32_t docId) = 0;
	virtual void OnOutDocUpdated(uint32_t docId) = 0;
	virtual void OnOutDocDeleted(uint32_t docId) = 0;
	virtual void OnOutDocLocked(uint32_t docId) = 0;
	virtual void OnOutDocUnlocked(uint32_t docId) = 0;

	virtual void OnBuyInvoiceInserted(uint32_t invoiceId) = 0;
	virtual void OnBuyInvoiceUpdated(uint32_t invoiceId) = 0;
	virtual void OnBuyInvoiceDeleted(uint32_t invoiceId) = 0;
	virtual void OnBuyInvoiceLocked(uint32_t invoiceId) = 0;
	virtual void OnBuyInvoiceUnlocked(uint32_t invoiceId) = 0;

	virtual void OnSellInvoiceInserted(uint32_t invoiceId) = 0;
	virtual void OnSellInvoiceUpdated(uint32_t invoiceId) = 0;
	virtual void OnSellInvoiceDeleted(uint32_t invoiceId) = 0;
	virtual void OnSellInvoiceLocked(uint32_t invoiceId) = 0;
	virtual void OnSellInvoiceUnlocked(uint32_t invoiceId) = 0;
};


#endif
