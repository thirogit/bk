#include "stdafx.h"
#include "CandidatesContent.h"
#include "../view/CowPresenceViewCreator.h"
#include <arrays/DeepDelete.h>
#include "../compare/DataComparator.h"
#include "../content/compare/RowComparator.h"
#include "../compare/CowPresenceViewComparator.h"

int CandidatesContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* CandidatesContent::GetRow(int row)
{
	return FindRow(row);
}

void CandidatesContent::AddRow(CowPresenceSelectRow* pRow)
{
	m_Rows.insert(m_Rows.begin(),pRow);
	NotifyCountChanged();
}

void CandidatesContent::AddRow(CowPresenceKey& key)
{	
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(m_herdId);
	ICowPresence* pPresence = pRegistry->GetCowPresence(key);
	CowPresenceViewCreator viewCreator(m_pSession);
		
	CowPresenceView* pContent = viewCreator.CreateView(pPresence);			
	CowPresenceSelectRow* pRow = new CowPresenceSelectRow(pContent);
	AddRow(pRow);		
}

void CandidatesContent::RemoveRow(CowPresenceKey& key)
{
	boost::multi_index::index<RowsIndex,tag_presenceKey>::type& index = m_Rows.get<tag_presenceKey>();
	auto rowIt = index.find(key);
	if(rowIt != index.end())
		index.erase(rowIt);	
	NotifyCountChanged();
}

void CandidatesContent::RemoveRow(int row)
{
	auto rowIt = (m_Rows.begin()+row);
	m_Rows.erase(rowIt);
	NotifyCountChanged();
}

void CandidatesContent::RemoveCow(uint32_t cowId)
{
	boost::multi_index::index<RowsIndex,tag_cowId>::type& index = m_Rows.get<tag_cowId>();
	auto rowIt = index.find(cowId);
	if(rowIt != index.end())
		index.erase(rowIt);	
	NotifyCountChanged();
}

CowPresenceSelectRow* CandidatesContent::FindRow(CowPresenceKey& key)
{
	boost::multi_index::index<RowsIndex,tag_presenceKey>::type& index = m_Rows.get<tag_presenceKey>();
	auto rowIt = index.find(key);
	if(rowIt != index.end())
	{
		return *rowIt;	
	}
	return NULL;
}

void CandidatesContent::ClearContent()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}
	

CowPresenceSelectRow* CandidatesContent::FindRow(int row)
{
	return *(m_Rows.begin()+row);
}

void CandidatesContent::CreateContent(SeasonSession* session,uint32_t herdId)
{
	m_herdId = herdId;
	m_pSession = session;	
		
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(m_herdId);
	PtrEnumerator<ICowPresence> cowEnum = pRegistry->EnumCows();

	CowPresenceViewCreator viewCreator(m_pSession);
	ICowPresence* pPresence;
	while(cowEnum.hasNext())
	{
		pPresence = *cowEnum;
		if(pPresence->IsInStock())
		{
			CowPresenceView* pContent = viewCreator.CreateView(pPresence);			
			CowPresenceSelectRow* pRow = new CowPresenceSelectRow(pContent);
			AddRow(pRow);
		}
		cowEnum.advance();
	}

	m_pSession->AddHerdObserver(m_herdId,this);
	
}

void CandidatesContent::OnHerdCowLocked(const CowPresenceKey& key)
{
}

void CandidatesContent::OnHerdCowUnlocked(const CowPresenceKey& key)
{
}

void CandidatesContent::OnHerdCowAdded(const CowPresenceKey& key)
{
}

void CandidatesContent::OnHerdCowRemoved(const CowPresenceKey& key)
{
}

void CandidatesContent::OnHerdCowMoved(const CowPresenceKey& fromKey,const CowPresenceKey& toKey)
{
}

void CandidatesContent::OnHerdCowUpdated(const CowPresenceKey& key)
{
}

void CandidatesContent::DestroyContent()
{
	ClearContent();	
	m_pSession->RemoveHerdObserver(m_herdId,this);
	m_herdId = NULL_ID;
	m_pSession = NULL;	
}

void CandidatesContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<CowPresenceViewComparator> > comparator(attributes);
	m_Rows.sort(comparator);
}