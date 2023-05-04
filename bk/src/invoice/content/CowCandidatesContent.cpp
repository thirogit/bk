#include "stdafx.h"
#include "CowCandidatesContent.h"
#include "../../view/CowViewCreator.h"
#include <arrays/DeepDelete.h>

int CowCandidatesContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* CowCandidatesContent::GetRow(int row)
{
	return GetRowAt(row);
}

void CowCandidatesContent::AddRow(CowSelectableRow* pRow)
{
	m_Rows.insert(m_Rows.begin(),pRow);
	NotifyCountChanged();
}

void CowCandidatesContent::AddCow(uint32_t cowId)
{	
	CowViewCreator viewCreator(m_pSession);

	CowClientObject* pCowCO = m_pSession->GetCow(cowId);
		
	CowView* pContent = viewCreator.CreateView(pCowCO->Object());	

	CowSelectableRow* pRow = new CowSelectableRow(pContent);
	AddRow(pRow);		
}

void CowCandidatesContent::RemoveRow(int row)
{
	auto rowIt = (m_Rows.begin()+row);
	m_Rows.erase(rowIt);
	NotifyCountChanged();
}

void CowCandidatesContent::RemoveCow(uint32_t cowId)
{
	boost::multi_index::index<RowsIndex,tag_cowId>::type& index = m_Rows.get<tag_cowId>();
	auto rowIt = index.find(cowId);
	if(rowIt != index.end())
		index.erase(rowIt);	
	NotifyCountChanged();
}


void CowCandidatesContent::ClearContent()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}
	

CowSelectableRow* CowCandidatesContent::GetRowAt(int row)
{
	return *(m_Rows.begin()+row);
}

void CowCandidatesContent::CreateContent(SeasonSession* session)
{
	m_pSession = session;		
}

void CowCandidatesContent::DestroyContent()
{
	ClearContent();	
	m_pSession = NULL;	
}

CowSelectableRow* CowCandidatesContent::FindRow(uint32_t cowId)
{
	boost::multi_index::index<RowsIndex,tag_cowId>::type&  index = m_Rows.get<tag_cowId>();
	auto rowIt = index.find(cowId);	
	if(rowIt != index.end())
	{
		return *rowIt;
	}
	return NULL;
}

void CowCandidatesContent::SortContent(const SortAttributes& attributes)
{
}
