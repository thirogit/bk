#ifndef __PURCHASEINVOICEDEDUCTIONVIEWCREATOR_H__
#define __PURCHASEINVOICEDEDUCTIONVIEWCREATOR_H__

#include <data\repo\IPurchaseInvoiceDeduction.h>
#include "PurchaseInvoiceDeductionView.h"

class PurchaseInvoiceDeductionViewCreator  
{
public:
	PurchaseInvoiceDeductionViewCreator();
	virtual void RefreshView(PurchaseInvoiceDeductionView* pView, IPurchaseInvoiceDeduction* pDeduction);
	virtual PurchaseInvoiceDeductionView* CreateView(IPurchaseInvoiceDeduction* pDeduction);
};


#endif