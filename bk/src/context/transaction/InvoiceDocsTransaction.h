#ifndef __INVOICEDOCSTRANSACTION_H__
#define __INVOICEDOCSTRANSACTION_H__

#include <boost/shared_ptr.hpp>
#include "ITransactionInterface.h"
#include "ObserverableInvoiceTransaction.h"
#include "TransactionState.h"
#include <data\impl\InvoiceDetails.h>
#include "InvoiceTransactionItem.h"
#include <data\runtime\InvoiceItemKey.h>
#include <data\impl\CowDetails.h>
#include <data\impl\CowInvoiceEntryDetails.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <data\runtime\InvoiceTransactionItemTuple.h>
#include "DocsScopeInvoiceTransactionFacade.h"
#include <data\impl\InvoiceHent.h>
#include "InvoiceItemIndex.h"
#include "DeductionItemIndex.h"
#include <data\runtime\DeductionItemTuple.h>

class InvoiceDocsTransaction : public ObserverableInvoiceTransaction
{
	class InvoiceTransactionFacadeImpl : public DocsScopeInvoiceTransactionFacade
	{
	public:
		InvoiceTransactionFacadeImpl(InvoiceDocsTransaction* pOwner);

		virtual const InvoiceDetails& GetDetails() const;
		virtual void UpdateDetails(const InvoiceDetails& details);	
		virtual void UpdateCow(ItemId id,const CowDetails& details);
		virtual void UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details);	
		virtual const InvoiceTransactionItem* GetItemAt(int index);
		virtual const InvoiceTransactionItem* GetItem(ItemId id);
		virtual count_t GetItemCount() const;
		virtual const InvoiceTransactionDeductionItem* GetDeductionAt(int index);
		virtual const InvoiceTransactionDeductionItem* GetDeduction(ItemId id);
		virtual count_t GetDeductionCount() const;
		virtual void UpdateDeduction(ItemId id, const InvoiceDeduction& deduction);
		virtual void AddObserver(InvoiceTransactionObserver* pObserver);
		virtual void RemoveObserver(InvoiceTransactionObserver* pObserver);
		virtual ItemId AddDeduction(const InvoiceDeduction& deduction);
		virtual void RemoveDeduction(ItemId id);
		virtual count_t GetDocCount();
		virtual uint32_t GetDocAt(int index);
		virtual InvoiceType GetInvoiceType() const;
	private:
		InvoiceDocsTransaction* m_pOwner;
	};

public:
	InvoiceDocsTransaction(ITransactionInterface* pInterface);
	~InvoiceDocsTransaction();
	void Open();	
		
	uint32_t GetInvoiceNo() const;

	InvoiceType GetInvoiceType() const;
	void SetInvoiceType(InvoiceType type);

	const InvoiceDetails& GetDetails() const;
	bool IsUpdating() const;		
	void UpdateDetails(const InvoiceDetails& details);
	
	count_t GetDocCount();
	uint32_t GetDocAt(int index);
	void AddDoc(uint32_t docId);		
	void DeleteDoc(uint32_t docId);	

	const InvoiceHent& GetHent() const;
	void SetHent(const InvoiceHent& hent);
	
	void UpdateCow(ItemId id,const CowDetails& details);
	void UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details);	
			
	const InvoiceTransactionItem* GetItemAt(int index);
	const InvoiceTransactionItem* GetItem(ItemId id);
	count_t GetItemCount() const;

	size_t GetDeductionCount() const;
	const InvoiceTransactionDeductionItem* GetDeduction(ItemId id);
	const InvoiceTransactionDeductionItem* GetDeductionAt(int index);
	void UpdateDeduction(ItemId id, const InvoiceDeduction& deduction);
	ItemId AddDeduction(const InvoiceDeduction& deduction);
	void RemoveDeduction(ItemId id);

		
	void Commit();
	void Rollback();
		
	DocsScopeInvoiceTransactionFacade* GetFacade();

protected:
	void ThrowIfNotOpen() const;
	InvoiceTransactionItem* CreateItemOutOf(const InvoiceTransactionItemTuple& itemTuple);
	InvoiceTransactionItem* FindItem(ItemId id);
	InvoiceTransactionDeductionItem* CreateDeductionItemOutOf(const DeductionItemTuple& itemTuple);
	InvoiceTransactionDeductionItem* AcquireDeduction(ItemId id);
private:
	InvoiceHent m_hent;
	InvoiceDetails m_details;
	TransactionState m_State;
	InvoiceType m_invoiceType;
	ITransactionInterface* m_pInterface;		
	uint32_t m_invoiceId;	
	uint32_t m_invoiceNo;	
	InvoiceItemIndex m_items;
	DeductionItemIndex m_deductions;
	InvoiceTransactionFacadeImpl* m_pFacade;
};

typedef boost::shared_ptr<InvoiceDocsTransaction> InvoiceDocsTransactionPtr;

#endif