#pragma once

#include "InvoicesListContent.h"

class SellInvoicesListContent : public InvoicesListContent
{
protected:
	virtual void CreateContent();	

	virtual void OnSellInvoiceInserted(uint32_t invoiceId);
	virtual void OnSellInvoiceUpdated(uint32_t invoiceId);
	virtual void OnSellInvoiceDeleted(uint32_t invoiceId);
	virtual void OnSellInvoiceLocked(uint32_t invoiceId);
	virtual void OnSellInvoiceUnlocked(uint32_t invoiceId);

};


