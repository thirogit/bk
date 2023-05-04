#include "stdafx.h"
#include "PurchaseIssuesListContent.h"
#include <arrays/DeepDelete.h>

IssueRow::IssueRow(DownloadIssueView* pView) : Row<DownloadIssueView>(pView)
{

}

std::wstring IssueRow::CellValue(uint32_t colId)
{
	return m_Text.Text(Content(), colId);
}

//----

PurchaseIssuesListContent::PurchaseIssuesListContent()
{
}

PurchaseIssuesListContent::~PurchaseIssuesListContent()
{	
	DeepDelete(m_Rows);
}

void PurchaseIssuesListContent::AddRow(IPurchaseDownloadIssue* issue)
{
	DownloadIssueView* view = m_viewCreator.CreateView(issue);
	IssueRow* row = new IssueRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}

int PurchaseIssuesListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* PurchaseIssuesListContent::GetRow(int row)
{
	return GetIssueRowAt(row);
}


IssueRow* PurchaseIssuesListContent::GetIssueRowAt(int row)
{
	return m_Rows[row];
}

void PurchaseIssuesListContent::SortContent(const SortAttributes& attributes)
{
}

void PurchaseIssuesListContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}