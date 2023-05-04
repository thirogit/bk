#ifndef __PURCHASEINVOICEVIEWCREATOR_H__
#define __PURCHASEINVOICEVIEWCREATOR_H__

#include <data\repo\IPurchaseInvoice.h>
#include "PurchaseInvoiceView.h"
#include "PurchaseHentViewCreator.h"

class PurchaseInvoiceViewCreator  
{
public:
	PurchaseInvoiceViewCreator();
	virtual void RefreshView(PurchaseInvoiceView* pView,IPurchaseInvoice* pPurchaseInvoice);
	virtual PurchaseInvoiceView* CreateView(IPurchaseInvoice* pPurchaseInvoice);
private:
	PurchaseHentViewCreator m_hentViewCreator;
};


#endif