#include "stdafx.h"
#include "HentsDocsCandidatesListContent.h"
#include "../../view/HentsDocViewCreator.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>

HentsDocsCandidatesListContent::HentsDocsCandidatesListContent() : m_pSession(NULL)
{
}

HentsDocsCandidatesListContent::~HentsDocsCandidatesListContent()
{
}

int HentsDocsCandidatesListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* HentsDocsCandidatesListContent::GetRow(int row)
{
	return GetDocAtRow(row);
}

SelectableHentsDocRow* HentsDocsCandidatesListContent::GetDocAtRow(int row)
{
	return m_Rows.row_at(row);
}
	
void HentsDocsCandidatesListContent::Create(SeasonSession* pSession)
{
	m_pSession = pSession;
	
	CreateContent();
	
	m_pSession->AddSeasonObserver(this);
}

void HentsDocsCandidatesListContent::AddDoc(uint32_t docId)
{
	if(m_Rows.find_row(docId) == NULL)
	{
		
		HentsDocViewCreator viewCreator(m_pSession);
		IHentsDoc* pDoc = GetHentsDoc(docId);
		HentsDocView* pView = viewCreator.CreateView(pDoc);
		SelectableHentsDocRow* pRow = new SelectableHentsDocRow(pView);
		m_Rows.add_row(pRow);
					
		NotifyCountChanged();
	}
}

void HentsDocsCandidatesListContent::RemoveDoc(uint32_t docId)
{
	SelectableHentsDocRow* pRemovedRow = m_Rows.remove_row(docId);
	delete pRemovedRow;
}

void HentsDocsCandidatesListContent::Destroy()
{
	if(m_pSession)
		m_pSession->RemoveSeasonObserver(this);

	DeepDelete(m_Rows);
	m_Rows.clear();
	m_pSession = NULL;

}

void HentsDocsCandidatesListContent::SortContent(const SortAttributes& attributes)
{
}