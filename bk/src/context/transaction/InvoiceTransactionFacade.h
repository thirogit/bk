#ifndef __INVOICETRANSACTIONFACADE_H__
#define __INVOICETRANSACTIONFACADE_H__

#include <data\impl\InvoiceDetails.h>
#include <data\runtime\ItemId.h>
#include <CommonTypes.h>
#include "InvoiceTransactionObserver.h"
#include "InvoiceTransactionDeductionItem.h"
#include "InvoiceTransactionItem.h"

class InvoiceTransactionFacade
{
public:
	virtual InvoiceType GetInvoiceType() const = 0;
	virtual const InvoiceDetails& GetDetails() const = 0;
	virtual void UpdateDetails(const InvoiceDetails& details) = 0;	
	virtual void UpdateCow(ItemId id,const CowDetails& details) = 0;
	virtual void UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details) = 0;	

	virtual const InvoiceTransactionItem* GetItemAt(int index) = 0;
	virtual const InvoiceTransactionItem* GetItem(ItemId id) = 0;
	virtual count_t GetItemCount() const = 0;

	virtual const InvoiceTransactionDeductionItem* GetDeductionAt(int index) = 0;
	virtual const InvoiceTransactionDeductionItem* GetDeduction(ItemId id) = 0;
	virtual count_t GetDeductionCount() const = 0;
	virtual void UpdateDeduction(ItemId id,const InvoiceDeduction& deduction) = 0;
	virtual ItemId AddDeduction(const InvoiceDeduction& deduction) = 0;
	virtual void RemoveDeduction(ItemId id) = 0;

	virtual void AddObserver(InvoiceTransactionObserver* pObserver) = 0;
	virtual void RemoveObserver(InvoiceTransactionObserver* pObserver) = 0;
};



#endif