#ifndef __PURCHASEINVOICECALCULATOR_H__
#define __PURCHASEINVOICECALCULATOR_H__

#include <data\repo\IPurchaseInvoice.h>

class PurchaseInvoiceCalculator
{
public:
	PurchaseInvoiceCalculator(IPurchaseInvoice* invoice);
	
	int GetCowCount();
	Decimal GetTotalWeight();
	Decimal GetTotalNet();
	Decimal GetTotalGross();
	
private:
	IPurchaseInvoice* m_invoice;

};

#endif