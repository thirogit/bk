#ifndef __INVOICETRANSACTIONOBSERVER_H__
#define __INVOICETRANSACTIONOBSERVER_H__

#include "TransactionObserver.h"

class InvoiceTransactionObserver : public TransactionObserver
{
public:
	virtual void OnDeductionUpdated(ItemId id) = 0;
	virtual void OnDeductionAdded(ItemId id) = 0;
	virtual void OnDeductionDeleted(ItemId id) = 0;
};



#endif