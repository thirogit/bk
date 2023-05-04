#ifndef __PURCHASEINVOICEDEDUCTIONVIEWTEXT_H__
#define __PURCHASEINVOICEDEDUCTIONVIEWTEXT_H__

#include "../view/PurchaseInvoiceDeductionView.h"

class PurchaseInvoiceDeductionViewText 
{
public:
	virtual std::wstring Text(const PurchaseInvoiceDeductionView *pView, uint32_t memberId);

};

#endif