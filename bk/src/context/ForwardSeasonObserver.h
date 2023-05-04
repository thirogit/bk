#ifndef __FORWARDSEASONOBSERVER_H__
#define __FORWARDSEASONOBSERVER_H__

#include "SeasonObserver.h"

class ForwardSeasonObserver : public SeasonObserver
{
public:
	
	virtual void  OnHentInserted(uint32_t) {};
	virtual void  OnHentUpdated(uint32_t) {};
	virtual void  OnHentDeleted(uint32_t) {};
	virtual void  OnHentLocked(uint32_t hentId) {};
	virtual void  OnHentUnlocked(uint32_t hentId) {};	

	virtual void  OnCowLocked(uint32_t) {};
	virtual void  OnCowInserted(uint32_t) {};
	virtual void  OnCowUpdated(uint32_t) {};
	virtual void  OnCowDeleted(uint32_t) {};
	virtual void  OnCowUnlocked(uint32_t) {};

	virtual void OnInDocInserted(uint32_t docId) {};
	virtual void OnInDocUpdated(uint32_t docId) {};
	virtual void OnInDocDeleted(uint32_t docId) {};
	virtual void OnInDocLocked(uint32_t docId) {};
	virtual void OnInDocUnlocked(uint32_t docId) {};

	virtual void OnBuyDocInserted(uint32_t docId) {};
	virtual void OnBuyDocUpdated(uint32_t docId) {};
	virtual void OnBuyDocDeleted(uint32_t docId) {};
	virtual void OnBuyDocLocked(uint32_t docId) {};
	virtual void OnBuyDocUnlocked(uint32_t docId) {};
	
	virtual void OnMoveDocUnlocked(uint32_t docId) {};
	virtual void OnMoveDocLocked(uint32_t docId) {};
	virtual void OnMoveDocInserted(uint32_t docId) {};
	virtual void OnMoveDocDeleted(uint32_t docId) {};
	virtual void OnMoveDocUpdated(uint32_t docId) {};

	virtual void OnSellDocLocked(uint32_t docId) {}
	virtual void OnSellDocInserted(uint32_t docId) {};
	virtual void OnSellDocUpdated(uint32_t docId) {};
	virtual void OnSellDocDeleted(uint32_t docId) {};
	virtual void OnSellDocUnlocked(uint32_t docId) {};	

	virtual void OnOutDocInserted(uint32_t docId) {};
	virtual void OnOutDocUpdated(uint32_t docId) {};
	virtual void OnOutDocDeleted(uint32_t docId) {};
	virtual void OnOutDocLocked(uint32_t docId) {};
	virtual void OnOutDocUnlocked(uint32_t docId) {};

	virtual void OnBuyInvoiceInserted(uint32_t invoiceId) {};
	virtual void OnBuyInvoiceUpdated(uint32_t invoiceId) {};
	virtual void OnBuyInvoiceDeleted(uint32_t invoiceId) {};
	virtual void OnBuyInvoiceLocked(uint32_t invoiceId) {};
	virtual void OnBuyInvoiceUnlocked(uint32_t invoiceId) {};

	virtual void OnSellInvoiceInserted(uint32_t invoiceId) {};
	virtual void OnSellInvoiceUpdated(uint32_t invoiceId) {};
	virtual void OnSellInvoiceDeleted(uint32_t invoiceId) {};
	virtual void OnSellInvoiceLocked(uint32_t invoiceId) {};
	virtual void OnSellInvoiceUnlocked(uint32_t invoiceId) {};
};


#endif