#ifndef __FATINVOICEVIEWCREATOR_H__
#define __FATINVOICEVIEWCREATOR_H__

#include "FatInvoiceView.h"
#include "InvoiceViewCreator.h"

class FatInvoiceViewCreator  
{
public:
	FatInvoiceViewCreator(InvoiceViewCreator* pThinViewCreator,SeasonSession* facade);
	FatInvoiceView* CreateView(const IInvoice* pInvoice);
	void RefreshView(FatInvoiceView* pView,const IInvoice* pInvoice);
private:
	SeasonSession* m_pFacade;
	InvoiceViewCreator* m_pThinViewCreator;

};


#endif