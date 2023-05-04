#include "stdafx.h"
#include "DocTransactionContent.h"
#include <arrays/DeepDelete.h>
#include "../view/DocItemViewCreator.h"

DocTransactionContent::DocTransactionContent() : m_pSession(NULL),m_transaction(NULL)
{
}

DocTransactionContent::~DocTransactionContent()
{	
}

void DocTransactionContent::Create(SeasonSession* pSession,DocTransactionFacade* transaction)
{
	m_pSession = pSession;
	m_transaction = transaction;
	
	DocItemView* view = NULL;
	const DocItem* item = NULL;
	DocItemViewCreator viewCreator(m_pSession);
	for(int i = 0,count = m_transaction->GetItemCount();i < count;i++)
	{
		item = m_transaction->GetItemAt(i);
		view = viewCreator.CreateView(item);				
		AddRow(new DocItemRow(view));
	}			
		

	m_transaction->AddObserver(this);
}

void DocTransactionContent::AddRow(DocItemRow* row)
{
	m_Rows.push_back(row);
}

int DocTransactionContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* DocTransactionContent::GetRow(int row)
{
	return GetItemRowAt(row);
}

void DocTransactionContent::Destroy()
{
	m_transaction->RemoveObserver(this);
	m_transaction = NULL;
	m_pSession = NULL;
	DeepDelete(m_Rows);	
	m_Rows.clear();
}

DocItemRow* DocTransactionContent::GetItemRowAt(int row)
{
	return m_Rows[row];
}

void DocTransactionContent::RefreshItemRow(ItemId itemId)
{		
	DocItemRow* row = FindRow(itemId);
	if(row != NULL)
	{
		DocItemViewCreator viewCreator(m_pSession);				
		viewCreator.RefreshView(m_transaction->GetItem(itemId),row->Content());		
	}			
}

void DocTransactionContent::OnItemUpdated(ItemId id)
{
	RefreshItemRow(id);
	NotifyContentUpdated();
}

void DocTransactionContent::OnItemAdded(ItemId id)
{
				
	DocItemViewCreator viewCreator(m_pSession);
	const DocItem* item = m_transaction->GetItem(id);
	DocItemView* view = viewCreator.CreateView(item);				
	AddRow(new DocItemRow(view));									
					
	NotifyCountChanged();
}

DocItemRow* DocTransactionContent::FindRow(ItemId id)
{
	boost::multi_index::index<TransactionContentIndex,index_tag_itemId>::type& indexByItemId = m_Rows.get<index_tag_itemId>();
	auto rowIt = indexByItemId.find(id);
	if(rowIt != indexByItemId.end())
	{
		return *rowIt;
	}
	return NULL;
}

void DocTransactionContent::OnItemDeleted(ItemId id)
{
	boost::multi_index::index<TransactionContentIndex,index_tag_itemId>::type& indexByItemId = m_Rows.get<index_tag_itemId>();
	auto rowIt = indexByItemId.find(id);
	if(rowIt != indexByItemId.end())
	{
		DocItemRow* pRow = *rowIt;
		indexByItemId.erase(rowIt);
		delete pRow;
		NotifyCountChanged();
	}
}

void DocTransactionContent::SortContent(const SortAttributes& attributes)
{
}
