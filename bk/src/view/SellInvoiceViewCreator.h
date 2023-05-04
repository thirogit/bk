#ifndef __SELLINVOICEVIEWCREATOR_H__
#define __SELLINVOICEVIEWCREATOR_H__

#include "InvoiceViewCreator.h"

class SellInvoiceViewCreator  : public InvoiceViewCreator  
{
public:
	SellInvoiceViewCreator(SeasonSession* facade);
	virtual void RefreshView(InvoiceView* pView,IInvoice* pInvoice);

};


#endif