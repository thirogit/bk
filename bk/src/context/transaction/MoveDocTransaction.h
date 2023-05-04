#ifndef __MOVEDOCTRANSACTION_H__
#define __MOVEDOCTRANSACTION_H__

#include "DocItem.h"
#include <data\impl\DocDetails.h>
#include <boost/shared_ptr.hpp>
#include <data\DocKey.h>
#include "MinusDocTransactionFacade.h"
#include <data\runtime\MoveDocOperations.h>
#include "ITransactionInterface.h"
#include "ObserverableTransaction.h"
#include "TransactionState.h"
#include "DocItemIndex.h"

class MoveDocTransaction;

class MoveDocTransactionFacade : public MinusDocTransactionFacade
{
public:
	MoveDocTransactionFacade(MoveDocTransaction* pTransaction);
	virtual const DocItem* GetItem(ItemId id) const;
	virtual const DocItem* GetItemAt(int index) const;
	virtual count_t GetItemCount() const;
	virtual void UpdateItemWeight(ItemId id,const NullDecimal& weight);
	virtual void UpdateItemGroup(ItemId id,const NullInt& group);
	virtual void UpdateItemStock(ItemId id,uint32_t stockId);
	virtual void UpdateItemClass(ItemId id,uint32_t classId);
	virtual void AddObserver(TransactionObserver* pObserver);
	virtual void RemoveObserver(TransactionObserver* pObserver);
	virtual void DeleteItem(ItemId id);
	virtual ItemId MinusCow(uint32_t cowId);
	
private:
	MoveDocTransaction* m_pTransaction;
};

class MoveDocTransaction : public ObserverableTransaction
{
public:
	MoveDocTransaction(ITransactionInterface* pInterface);
	~MoveDocTransaction();
	void Open();	
	
	virtual const DocDetails& GetDetails() const;
	virtual bool IsUpdating() const;		

	virtual void UpdateDetails(const DocDetails& details);
	virtual ItemId AddCow(uint32_t cowId);	
	virtual void UpdateCowEntry(ItemId,const CowEntryDetails& details);	
	virtual void DeleteCow(ItemId id);	
	virtual const DocItem* GetItemAt(int index);
	virtual const DocItem* GetItem(ItemId id);
	virtual count_t GetItemCount() const;
	virtual void Commit();
	virtual void Rollback();
	virtual DocKey GetDocKey() const;
	virtual MinusDocTransactionFacade* GetFacade();
	virtual uint32_t GetSrcHerdId();
	virtual uint32_t GetDstHerdId();
	
protected:
	void ThrowIfNotOpen() const;
	DocItem* AcquireItem(ItemId id);
	void EraseItem(ItemId id);
private:
	uint32_t m_srcHerdId;
	uint32_t m_dstHerdId;
	DocDetails m_details;
	TransactionState m_State;
	ITransactionInterface* m_pInterface;
	uint32_t m_docId;
	DocItemIndex m_items;
	MoveDocTransactionFacade* m_pFacade;
};

typedef boost::shared_ptr<MoveDocTransaction> MoveDocTransactionPtr;

#endif