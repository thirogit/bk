#ifndef __LISTCONTENT_H__
#define __LISTCONTENT_H__

#include "ContentObserver.h"
#include <set>
#include "ListRow.h"
#include <functional>
#include "../../compare/SortAttributes.h"

class ListContent
{
public:
	void AddContentObserver(ContentObserver* pObserver);
	void RemoveContentObserver(ContentObserver* pObserver);

	virtual int GetRowCount() const = 0;
	virtual ListRow* GetRow(int row) = 0;
	virtual void SortContent(const SortAttributes& attributes) = 0;
protected:
	void NotifyBeforeUpdate();
	void NotifyAfterUpdate();
	void NotifyCountChanged();
	void NotifyContentUpdated();
	void InvokeOnObservers(std::function<void (ContentObserver*)>& f);
private:
	std::set<ContentObserver*> m_Observers;
};


#endif