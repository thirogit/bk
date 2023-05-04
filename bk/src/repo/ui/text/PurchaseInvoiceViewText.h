#ifndef __PURCHASEINVOICEVIEWTEXT_H__
#define __PURCHASEINVOICEVIEWTEXT_H__

#include "../view/PurchaseInvoiceView.h"

class PurchaseInvoiceViewText 
{
public:
	virtual std::wstring Text(const PurchaseInvoiceView *pView, uint32_t memberId);

};

#endif