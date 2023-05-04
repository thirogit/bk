#ifndef __DOCSCOPEINVOICETRANSACTIONFACADE_H__
#define __DOCSCOPEINVOICETRANSACTIONFACADE_H__


#include "InvoiceTransactionFacade.h"

class DocsScopeInvoiceTransactionFacade : public InvoiceTransactionFacade
{
public:
	virtual count_t GetDocCount() = 0;
	virtual uint32_t GetDocAt(int index) = 0;
};



#endif