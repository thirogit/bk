#ifndef __TRANSACTIONOBSERVER_H__
#define __TRANSACTIONOBSERVER_H__

#include <data\runtime\ItemId.h>

class TransactionObserver
{
public:
	virtual void OnItemUpdated(ItemId id) = 0;
	virtual void OnItemAdded(ItemId id) = 0;
	virtual void OnItemDeleted(ItemId id) = 0;
};



#endif