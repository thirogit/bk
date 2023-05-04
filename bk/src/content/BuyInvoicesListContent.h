#pragma once

#include "InvoicesListContent.h"

class BuyInvoicesListContent : public InvoicesListContent
{
protected:
	virtual void CreateContent();

	virtual void OnBuyInvoiceInserted(uint32_t invoiceId);
	virtual void OnBuyInvoiceUpdated(uint32_t invoiceId);
	virtual void OnBuyInvoiceDeleted(uint32_t invoiceId);
	virtual void OnBuyInvoiceLocked(uint32_t invoiceId);
	virtual void OnBuyInvoiceUnlocked(uint32_t invoiceId);


};


