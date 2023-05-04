#ifndef __OBSERVERABLEINVIOCETRANSACTION_H__
#define __OBSERVERABLEINVIOCETRANSACTION_H__

#include <data\runtime\ItemId.h>
#include "InvoiceTransactionObserver.h"
#include <set>
#include <algorithm>

class ObserverableInvoiceTransaction 
{
public:
	void NotifyItemUpdated(ItemId id);
	void NotifyItemAdded(ItemId id);
	void NotifyItemDeleted(ItemId id);
	
	void NotifyDeductionUpdated(ItemId id);
	void NotifyDeductionAdded(ItemId id);
	void NotifyDeductionDeleted(ItemId id);

	void AddObserver(InvoiceTransactionObserver* pObserver);
	void RemoveObserver(InvoiceTransactionObserver* pObserver);
protected:
	template<typename F>
	void VisitObservers(F& f)
	{
		std::for_each(m_Observers.begin(),m_Observers.end(),f);
	}
private:
	std::set<InvoiceTransactionObserver*> m_Observers;
};

#endif