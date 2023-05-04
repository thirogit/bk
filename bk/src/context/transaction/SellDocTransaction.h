#ifndef __SELLDOCTRANSACTION_H__
#define __SELLDOCTRANSACTION_H__

#include "DocItem.h"
#include <data\impl\DocDetails.h>
#include <boost/shared_ptr.hpp>
#include <data\DocKey.h>
#include "MinusDocTransactionFacade.h"
#include "ITransactionInterface.h"
#include "ObserverableTransaction.h"
#include "TransactionState.h"
#include "DocItemIndex.h"

class SellDocTransaction;

class SellDocTransactionFacade : public MinusDocTransactionFacade
{
public:
	SellDocTransactionFacade(SellDocTransaction* pTransaction);
	virtual const DocItem* GetItem(ItemId id) const;
	virtual const DocItem* GetItemAt(int index) const;
	virtual count_t GetItemCount() const;
	virtual void UpdateItemWeight(ItemId id,const NullDecimal& weight);
	virtual void UpdateItemGroup(ItemId id,const NullInt& group);
	virtual void UpdateItemStock(ItemId id,uint32_t stockId);
	virtual void UpdateItemClass(ItemId id,uint32_t classId);
	virtual void AddObserver(TransactionObserver* pObserver);
	virtual void RemoveObserver(TransactionObserver* pObserver);
	virtual ItemId MinusCow(uint32_t cowId);
	virtual void DeleteItem(ItemId id);	
private:
	SellDocTransaction* m_pTransaction;
};

class SellDocTransaction : public ObserverableTransaction
{
public:
	SellDocTransaction(ITransactionInterface* pInterface);
	~SellDocTransaction();
	void Open();	
	
	virtual const DocDetails& GetDetails() const;
	virtual bool IsUpdating() const;		
	virtual void UpdateDetails(const DocDetails& details);
	virtual ItemId AddCow(uint32_t cowId);	
	virtual void UpdateCowEntry(ItemId id,const CowEntryDetails& details);	
			void SetHent(uint32_t hentId);
		uint32_t GetHent() const;
	virtual void DeleteCow(ItemId id);		
	virtual const DocItem* GetItemAt(int index);
	virtual const DocItem* GetItem(ItemId id);
			
	virtual count_t GetItemCount() const;
	virtual void Commit();
	virtual void Rollback();
	virtual DocKey GetDocKey() const;
	uint32_t GetHerdId() const;
	
	virtual MinusDocTransactionFacade* GetFacade();
protected:
	void ThrowIfNotOpen() const;
	DocItem* AcquireItem(ItemId id);
	DocItem* FindItem(ItemId id);
private:
	DocDetails m_details;
	TransactionState m_State;
	ITransactionInterface* m_pInterface;
	uint32_t m_docId;
	uint32_t m_hentId;
	uint32_t m_herdId;
	DocItemIndex m_items;

	SellDocTransactionFacade* m_pFacade;
};

typedef boost::shared_ptr<SellDocTransaction> SellDocTransactionPtr;

#endif