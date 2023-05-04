#include "stdafx.h"
#include "SimpleHentsDocsListContent.h"
#include "../../view/HentsDocViewCreator.h"
#include <arrays\DeepDelete.h>

SimpleHentsDocsListContent::SimpleHentsDocsListContent(SeasonSession* pSession) : m_pSession(pSession) 
{
}

SimpleHentsDocsListContent::~SimpleHentsDocsListContent()
{
	RemoveAllDocs();
}

int SimpleHentsDocsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* SimpleHentsDocsListContent::GetRow(int row)
{
	return GetDocAtRow(row);
}

HentsDocRow* SimpleHentsDocsListContent::GetDocAtRow(int row)
{
	return m_Rows.row_at(row);
}

void SimpleHentsDocsListContent::AddDoc(IHentsDoc* pDoc)
{
	HentsDocViewCreator viewCreator(m_pSession);
	m_Rows.add_row(new HentsDocRow(viewCreator.CreateView(pDoc)));
	NotifyCountChanged();
}

void SimpleHentsDocsListContent::RemoveDoc(uint32_t docId)
{
	HentsDocRow* pRemovedDoc = m_Rows.find_row(docId);
	delete pRemovedDoc;
	NotifyCountChanged();
}

void SimpleHentsDocsListContent::RemoveAllDocs()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
	NotifyCountChanged();
}

void SimpleHentsDocsListContent::SortContent(const SortAttributes& attributes)
{
}
