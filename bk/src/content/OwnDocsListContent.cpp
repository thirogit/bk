#include "stdafx.h"
#include "OwnDocsListContent.h"
#include "../view/OwnDocViewCreator.h"
#include <arrays\DeepDelete.h>

#include "../compare/DataComparator.h"
#include "compare/RowComparator.h"
#include "../compare/OwnDocComparator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

OwnDocsListContent::OwnDocsListContent() : m_pSession(NULL),m_herdId(NULL_ID)
{
}

OwnDocsListContent::~OwnDocsListContent()
{	
}

uint32_t OwnDocsListContent::GetHerdId() const
{
	return m_herdId;
}

void OwnDocsListContent::Create(SeasonSession* pSession,uint32_t herdId)
{
	m_pSession = pSession;
	m_herdId = herdId;
	CreateContent();
	m_pSession->AddHerdObserver(m_herdId,this);	
}

void OwnDocsListContent::AddRow(OwnDocRow* row)
{
	m_Rows.add_row(row);
}

int OwnDocsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* OwnDocsListContent::GetRow(int row)
{
	return GetDocAtRow(row);
}

OwnDocRow* OwnDocsListContent::GetDocAtRow(int row)
{
	return m_Rows.row_at(row);
}

void OwnDocsListContent::Destroy()
{
	if(m_pSession)
		m_pSession->RemoveHerdObserver(m_herdId,this);

	m_pSession = NULL;
	m_herdId = NULL_ID;
	DeepDelete(m_Rows);	
}

void OwnDocsListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<OwnDocComparator> > comparator(attributes);
	m_Rows.sort(comparator);
}