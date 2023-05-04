#ifndef __BUYINVOICEVIEWCREATOR_H__
#define __BUYINVOICEVIEWCREATOR_H__

#include "InvoiceViewCreator.h"
#include <data\IInvoice.h>
#include "../context/SeasonSession.h"

class BuyInvoiceViewCreator  : public InvoiceViewCreator
{
public:
	BuyInvoiceViewCreator(SeasonSession* facade);
	virtual void RefreshView(InvoiceView* pView,IInvoice* pInvoice);	
private:
	SeasonSession* m_pFacade;
};


#endif