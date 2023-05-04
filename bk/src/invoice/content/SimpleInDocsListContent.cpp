#include "stdafx.h"
#include "SimpleInDocsListContent.h"
#include "../../view/OwnDocViewCreator.h"
#include <arrays\DeepDelete.h>

SimpleInDocsListContent::SimpleInDocsListContent(SeasonSession* pSession) : m_pSession(pSession) 
{
}

SimpleInDocsListContent::~SimpleInDocsListContent()
{
	RemoveAllDocs();
}

int SimpleInDocsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* SimpleInDocsListContent::GetRow(int row)
{
	return GetDocAtRow(row);
}

OwnDocRow* SimpleInDocsListContent::GetDocAtRow(int row)
{
	return m_Rows.row_at(row);
}

void SimpleInDocsListContent::AddDoc(IOwnDoc* pDoc)
{
	OwnDocViewCreator viewCreator(m_pSession);
	m_Rows.add_row(new OwnDocRow(viewCreator.CreateView(pDoc)));
	NotifyCountChanged();
}

void SimpleInDocsListContent::RemoveDoc(uint32_t docId)
{
	OwnDocRow* pRemovedDoc = m_Rows.find_row(docId);
	delete pRemovedDoc;
	NotifyCountChanged();
}

void SimpleInDocsListContent::RemoveAllDocs()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
	NotifyCountChanged();
}


void SimpleInDocsListContent::SortContent(const SortAttributes& attributes)
{
}