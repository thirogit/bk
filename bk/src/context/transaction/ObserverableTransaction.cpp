#include "stdafx.h"
#include "ObserverableTransaction.h"

void ObserverableTransaction::NotifyItemUpdated(ItemId id)
{
	VisitObservers([id](TransactionObserver* pObserver)
					{
						pObserver->OnItemUpdated(id);
					});
}

void ObserverableTransaction::NotifyItemAdded(ItemId id)
{
	VisitObservers([id](TransactionObserver* pObserver)
					{
						pObserver->OnItemAdded(id);
					});
}

void ObserverableTransaction::NotifyItemDeleted(ItemId id)
{
	VisitObservers([id](TransactionObserver* pObserver)
					{
						pObserver->OnItemDeleted(id);
					});
}

void ObserverableTransaction::AddObserver(TransactionObserver* pObserver)
{
	m_Observers.insert(pObserver);
}

void ObserverableTransaction::RemoveObserver(TransactionObserver* pObserver)
{
	m_Observers.erase(pObserver);
};

