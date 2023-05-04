#include "stdafx.h"
#include "ListContent.h"

void ListContent::RemoveContentObserver(ContentObserver* pObserver)
{
	m_Observers.erase(pObserver);
}

void ListContent::AddContentObserver(ContentObserver* pObserver)
{
	m_Observers.insert(pObserver);
}

void ListContent::InvokeOnObservers(std::function<void (ContentObserver*)>& f)
{
	auto observerIt = m_Observers.begin();
	while(observerIt != m_Observers.end())
	{
		f(*observerIt);
		observerIt++;
	}
}

void ListContent::NotifyBeforeUpdate()
{
	std::function<void (ContentObserver*)> notify = [](ContentObserver* pObserver)
	{
		pObserver->OnBeforeUpdate();
	};
	InvokeOnObservers(notify);
}

void ListContent::NotifyAfterUpdate()
{
	std::function<void (ContentObserver*)> notify = [](ContentObserver* pObserver)
	{
		pObserver->OnAfterUpdate();
	};
	InvokeOnObservers(notify);
}

void ListContent::NotifyCountChanged()
{
	std::function<void (ContentObserver*)> notify = [](ContentObserver* pObserver)
	{
		pObserver->OnCountChanged();
	};
	InvokeOnObservers(notify);
}

void ListContent::NotifyContentUpdated()
{
	std::function<void (ContentObserver*)> notify = [](ContentObserver* pObserver)
	{
		pObserver->OnContentChanged();
	};
	InvokeOnObservers(notify);
}
