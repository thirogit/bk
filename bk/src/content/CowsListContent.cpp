#include "stdafx.h"
#include "CowsListContent.h"
#include <arrays/DeepDelete.h>
#include "../view/CowPresenceViewCreator.h"
#include "../compare/CowPresenceViewComparator.h"
#include "../compare/DataComparator.h"
#include "compare/RowComparator.h"
#include "../compare/CowPresenceViewComparator.h"
#include <boost/foreach.hpp>

CowsListContent::CowsListContent() : m_pSession(NULL)
{
}

CowsListContent::~CowsListContent()
{	
}

uint32_t CowsListContent::GetHerdId() const
{
	return m_herdId;
}

void CowsListContent::Create(SeasonSession* pSession,uint32_t herdId)
{
	m_pSession = pSession;
	m_herdId = herdId;
	CreateContent();		
	m_pSession->AddHerdObserver(herdId,this);														
}

void CowsListContent::AddRow(CowPresenceRow* row)
{
	m_Rows.push_back(row);		
}

int CowsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* CowsListContent::GetRow(int row)
{
	return m_Rows[row];
}

void CowsListContent::Destroy()
{
	m_pSession = NULL;
	DeepDelete(m_Rows);	
}

CowPresenceRow* CowsListContent::GetCowAtRow(int row)
{
	return m_Rows[row];
}

void CowsListContent::RefreshCowRow(const CowPresenceKey& rowKey)
{
	boost::multi_index::index<IndexType,tag_Key>::type&  index = m_Rows.get<tag_Key>();
	auto rowIt = index.find(rowKey);	
		
	if(rowIt != index.end())
	{		
		CowPresenceRow* pRow = *rowIt;
		
		HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(m_herdId);
		CowPresenceViewCreator viewCreator(m_pSession);
		CowPresenceView* pView = pRow->Content();			
		ICowPresence* pHerdPresence = pRegistry->GetCowPresence(pView->GetKey());			
		viewCreator.RefreshView(pView,pHerdPresence);		
		NotifyContentUpdated();		
	}
}

void CowsListContent::OnHerdCowLocked(const CowPresenceKey& rowKey)
{
	RefreshCowRow(rowKey);
}

void CowsListContent::OnHerdCowUnlocked(const CowPresenceKey& rowKey)
{
	RefreshCowRow(rowKey);
}

void CowsListContent::OnHerdCowUpdated(const CowPresenceKey& key)
{
	RefreshCowRow(key);
}

void CowsListContent::OnHerdCowAdded(const CowPresenceKey& key)
{
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(m_herdId);
	CowPresenceViewCreator viewCreator(m_pSession);
	ICowPresence* pPresence = pRegistry->GetCowPresence(key);
	CowPresenceView* pCowView = viewCreator.CreateView(pPresence);
	AddRow(new CowPresenceRow(pCowView));	
	NotifyCountChanged();
}

void CowsListContent::OnHerdCowDeleted(const CowPresenceKey& key)
{
	RemoveRow(key);
}

void CowsListContent::RemoveRow(const CowPresenceKey& rowKey)
{
	boost::multi_index::index<IndexType,tag_Key>::type&  index = m_Rows.get<tag_Key>();
	auto rowIt = index.find(rowKey);	
		
	if(rowIt != index.end())
	{	
		index.erase(rowKey);
	}
	NotifyCountChanged();
	
}

void CowsListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<CowPresenceViewComparator> > comparator(attributes);
	m_Rows.sort(comparator);
}

FindResult CowsListContent::Find(Predicate<CowPresenceView>* predicate)
{
	FindResult result;
	result.count = 0;
	
	int i = 0;
	BOOST_FOREACH(CowPresenceRow* row, m_Rows)
	{
		row->SetIsMarked(false);
		if (predicate->Test(row->Content()))
		{
			result.count++;
			row->SetIsMarked(true);

			if (result.firstOccurence < 0)
			{
				result.firstOccurence = i;
			}
		}
		i++;
	}

	SortFound();

	NotifyContentUpdated();
	return result;
}

void CowsListContent::ClearFound()
{
	BOOST_FOREACH(CowPresenceRow* row, m_Rows)
	{
		row->SetIsMarked(false);
	}
	NotifyContentUpdated();
}

void CowsListContent::SortFound()
{
	for (int i = 0, s = m_Rows.size(); i < s; i++)
	{
		auto it = m_Rows.begin() + i;

		if ((*it)->GetIsMarked())
		{
			m_Rows.relocate(m_Rows.begin(), it);
		}
	}
}