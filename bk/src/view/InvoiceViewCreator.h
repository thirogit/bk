#ifndef __INVOICEVIEWCREATOR_H__
#define __INVOICEVIEWCREATOR_H__

#include "InvoiceView.h"
#include <data\IInvoice.h>
#include "../context/SeasonSession.h"

class InvoiceViewCreator  
{
public:
	InvoiceViewCreator(SeasonSession* facade);
	virtual void RefreshView(InvoiceView* pView,IInvoice* pInvoice);
	virtual InvoiceView* CreateView(IInvoice* pInvoice);
protected:
	SeasonSession* m_pFacade;
};


#endif