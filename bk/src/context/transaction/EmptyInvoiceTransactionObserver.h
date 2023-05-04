#ifndef __EMPTYINVOICETRANSACTIONOBSERVER_H__
#define __EMPTYINVOICETRANSACTIONOBSERVER_H__

#include "InvoiceTransactionObserver.h"

class EmptyInvoiceTransactionObserver : public InvoiceTransactionObserver
{
public:
	virtual void OnItemUpdated(ItemId id) {};
	virtual void OnItemAdded(ItemId id) {};
	virtual void OnItemDeleted(ItemId id) {};
	virtual void OnDeductionUpdated(ItemId id) {};
	virtual void OnDeductionAdded(ItemId id) {};
	virtual void OnDeductionDeleted(ItemId id) {};
};



#endif