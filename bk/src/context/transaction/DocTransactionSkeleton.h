#ifndef __DOCTRANSACTIONSKELETON_H__
#define __DOCTRANSACTIONSKELETON_H__

#include "DocItem.h"
#include <data\impl\DocDetails.h>
#include "ITransactionInterface.h"
#include "ObserverableTransaction.h"
#include "TransactionState.h"
#include "ItemIndex.h"

class DocTransactionSkeleton : public ObserverableTransaction
{
public:
	DocTransactionSkeleton(ITransactionInterface* pInterface);
	void Open();	
		
	void Commit();
	void Rollback();
	
	size_t GetItemCount() const;	
	const DocItem* GetItem(ItemId id);
	const DocItem* GetItemAt(int index);

protected:
	virtual void Init() = 0;
	uint32_t GetDocId() const;
	void SetDocId(uint32_t docId);
	boost::any ExecuteOperation(uint32_t operation,OperationArguments& args);	
	void AddItem(DocItem* pItem);
	void RemoveItem(ItemId id);
	DocItem* AcquireItem(ItemId id);
	
protected:
	void ThrowIfNotOpen() const;	
private:
	DocDetails m_details;
	TransactionState m_State;
	ITransactionInterface* m_pInterface;
	ItemIndex m_items;
	uint32_t m_docId;
};



#endif