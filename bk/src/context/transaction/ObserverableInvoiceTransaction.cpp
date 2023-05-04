#include "stdafx.h"
#include "ObserverableInvoiceTransaction.h"
#include <functional>

void ObserverableInvoiceTransaction::NotifyItemUpdated(ItemId id)
{
	VisitObservers([id](InvoiceTransactionObserver* pObserver)
					{
						pObserver->OnItemUpdated(id);
					});
}

void ObserverableInvoiceTransaction::NotifyItemAdded(ItemId id)
{
	VisitObservers([id](InvoiceTransactionObserver* pObserver)
					{
						pObserver->OnItemAdded(id);
					});
}

void ObserverableInvoiceTransaction::NotifyItemDeleted(ItemId id)
{
	VisitObservers([id](InvoiceTransactionObserver* pObserver)
					{
						pObserver->OnItemDeleted(id);
					});
}

void ObserverableInvoiceTransaction::NotifyDeductionUpdated(ItemId id)
{
	auto fn = std::mem_fn(&InvoiceTransactionObserver::OnDeductionUpdated);
	std::function<void(InvoiceTransactionObserver*)> visitor = std::bind(fn, std::placeholders::_1, id);
	VisitObservers(visitor);
}

void ObserverableInvoiceTransaction::NotifyDeductionAdded(ItemId id)
{
	auto fn = std::mem_fn(&InvoiceTransactionObserver::OnDeductionAdded);
	std::function<void(InvoiceTransactionObserver*)> visitor = std::bind(fn, std::placeholders::_1, id);
	VisitObservers(visitor);

}

void ObserverableInvoiceTransaction::NotifyDeductionDeleted(ItemId id)
{
	auto fn = std::mem_fn(&InvoiceTransactionObserver::OnDeductionDeleted);
	std::function<void(InvoiceTransactionObserver*)> visitor = std::bind(fn, std::placeholders::_1, id);
	VisitObservers(visitor);
}


void ObserverableInvoiceTransaction::AddObserver(InvoiceTransactionObserver* pObserver)
{
	m_Observers.insert(pObserver);
}

void ObserverableInvoiceTransaction::RemoveObserver(InvoiceTransactionObserver* pObserver)
{
	m_Observers.erase(pObserver);
};

