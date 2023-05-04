#include "stdafx.h"
#include "HentsDocsListContent.h"
#include "../view/OwnDocViewCreator.h"
#include <arrays/DeepDelete.h>

#include "../compare/DataComparator.h"
#include "compare/RowComparator.h"
#include "../compare/HentsDocComparator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HentsDocsListContent::HentsDocsListContent() : m_pSession(NULL),m_herdId(NULL_ID)
{
}

HentsDocsListContent::~HentsDocsListContent()
{	
}

void HentsDocsListContent::Create(SeasonSession* pSession,uint32_t herdId)
{
	m_pSession = pSession;
	m_herdId = herdId;
			
	CreateContent();
	m_pSession->AddHerdObserver(m_herdId,this);
	
}

uint32_t HentsDocsListContent::GetHerdId() const
{
	return m_herdId;
}

void HentsDocsListContent::AddRow(HentsDocRow* row)
{
	m_Rows.add_row(row);
}

int HentsDocsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* HentsDocsListContent::GetRow(int row)
{
	return GetDocAtRow(row);
}

void HentsDocsListContent::RemoveRow(uint32_t docId)
{
	HentsDocRow* pRemovedRow = m_Rows.remove_row(docId);
	delete pRemovedRow;
}

void HentsDocsListContent::Destroy()
{
	if(m_pSession)
		m_pSession->RemoveHerdObserver(m_herdId,this);
		
	m_pSession = NULL;
	m_herdId = NULL_ID;
	DeepDelete(m_Rows);
	
}

HentsDocRow* HentsDocsListContent::GetDocAtRow(int row)
{
	return m_Rows.row_at(row);
}

void HentsDocsListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<HentsDocComparator> > comparator(attributes);
	m_Rows.sort(comparator);
}