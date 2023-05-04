#ifndef __DOCTRANSACTIONFACADE_H__
#define __DOCTRANSACTIONFACADE_H__

#include "DocItem.h"
#include "TransactionObserver.h"
#include <CommonTypes.h>

class DocTransactionFacade
{
public:
	virtual const DocItem* GetItem(ItemId id) const = 0;
	virtual const DocItem* GetItemAt(int index) const = 0;
	virtual count_t GetItemCount() const = 0;
	
	virtual void DeleteItem(ItemId id) = 0;
	
	virtual void UpdateItemWeight(ItemId id,const NullDecimal& weight) = 0;
	virtual void UpdateItemGroup(ItemId id,const NullInt& group) = 0;
	virtual void UpdateItemStock(ItemId id,uint32_t stockId) = 0;
	virtual void UpdateItemClass(ItemId id,uint32_t classId) = 0;
	
	virtual void AddObserver(TransactionObserver* pObserver) = 0;
	virtual void RemoveObserver(TransactionObserver* pObserver) = 0;
};

#endif