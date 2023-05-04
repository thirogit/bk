#ifndef __BUYDOCTRANSACTION_H__
#define __BUYDOCTRANSACTION_H__

#include "DocItem.h"
#include <data\impl\DocDetails.h>
#include <boost/shared_ptr.hpp>
#include <data\DocKey.h>
#include "PlusDocTransactionFacade.h"
#include <data\runtime\BuyDocOperations.h>
#include "ITransactionInterface.h"
#include "ObserverableTransaction.h"
#include "TransactionState.h"
#include "DocItemIndex.h"

class BuyDocTransaction;

class BuyDocTransactionFacade : public PlusDocTransactionFacade
{
public:
	BuyDocTransactionFacade(BuyDocTransaction* pTransaction);
	virtual const DocItem* GetItem(ItemId id) const;
	virtual const DocItem* GetItemAt(int index) const;
	virtual count_t GetItemCount() const;
	virtual void UpdateItemWeight(ItemId id,const NullDecimal& weight);
	virtual void UpdateItemGroup(ItemId id,const NullInt& group);
	virtual void UpdateItemStock(ItemId id,uint32_t stockId);
	virtual void UpdateItemClass(ItemId id,uint32_t classId);
	virtual void AddObserver(TransactionObserver* pObserver);
	virtual void RemoveObserver(TransactionObserver* pObserver);
	virtual void UpdateCow(ItemId id,const CowDetails& details);
	virtual ItemId PlusCow(const CowDetails& details);
	virtual void DeleteItem(ItemId id);
private:
	BuyDocTransaction* m_pTransaction;
};

class BuyDocTransaction : public ObserverableTransaction
{
public:
	BuyDocTransaction(ITransactionInterface* pInterface);
	~BuyDocTransaction();
	void Open();	
	
	virtual const DocDetails& GetDetails() const;
	virtual bool IsUpdating() const;		
	virtual void UpdateDetails(const DocDetails& details);
	virtual ItemId AddCow(const CowDetails& details);		
	
	virtual void UpdateCow(ItemId id,const CowDetails& details);
	virtual void UpdateCowEntry(ItemId id,const CowEntryDetails& details);		
	virtual void DeleteCow(ItemId id);	

			
	virtual const DocItem* GetItemAt(int index);
	virtual const DocItem* GetItem(ItemId id);
	virtual count_t GetItemCount() const;
	virtual void Commit();
	virtual void Rollback();
	virtual DocKey GetDocKey() const;
			uint32_t GetHerdId() const;
			uint32_t GetHentId() const;
			void SetHent(uint32_t hentId);
	virtual PlusDocTransactionFacade* GetFacade();
protected:
	void ThrowIfNotOpen() const;
	DocItem* AcquireItem(ItemId id);
	void EraseItem(ItemId id);
	
private:
	DocDetails m_details;
	TransactionState m_State;
	ITransactionInterface* m_pInterface;
	uint32_t m_docId;
	uint32_t m_herdId;
	uint32_t m_hentId;
	DocItemIndex m_items;
	BuyDocTransactionFacade* m_pFacade;
};

typedef boost::shared_ptr<BuyDocTransaction> BuyDocTransactionPtr;

#endif