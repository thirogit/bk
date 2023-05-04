#ifndef __COWSCOPEINVOICETRANSACTIONFACADE_H__
#define __COWSCOPEINVOICETRANSACTIONFACADE_H__


#include "InvoiceTransactionFacade.h"

class CowsScopeInvoiceTransactionFacade : public InvoiceTransactionFacade
{
public:
	virtual ItemId AddCow(uint32_t cowId) = 0;
	virtual void DeleteCow(uint32_t cowId) = 0;
	virtual const InvoiceTransactionItem* GetItem(uint32_t cowId) = 0;
	virtual uint32_t GetDocId() const = 0;
};



#endif