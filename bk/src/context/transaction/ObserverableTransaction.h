#ifndef __OBSERVERABLETRANSACTION_H__
#define __OBSERVERABLETRANSACTION_H__

#include <data\runtime\ItemId.h>
#include "TransactionObserver.h"
#include <set>
#include <algorithm>

class ObserverableTransaction 
{
public:
	void NotifyItemUpdated(ItemId id);
	void NotifyItemAdded(ItemId id);
	void NotifyItemDeleted(ItemId id);

	void AddObserver(TransactionObserver* pObserver);
	void RemoveObserver(TransactionObserver* pObserver);
protected:
	template<typename F>
	void VisitObservers(F& f)
	{
		std::for_each(m_Observers.begin(),m_Observers.end(),f);
	}
private:
	std::set<TransactionObserver*> m_Observers;
};

#endif