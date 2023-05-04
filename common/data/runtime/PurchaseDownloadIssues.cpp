#include "stdafx.h"
#include "PurchaseDownloadIssues.h"
#include <arrays/DeepDelete.h>

PurchaseDownloadIssues::PurchaseDownloadIssues()
{
}

PurchaseDownloadIssues::~PurchaseDownloadIssues()
{
	DeepDelete(m_issues);
}

PurchaseDownloadIssues::PurchaseDownloadIssues(const PurchaseDownloadIssues& src)
{
	operator=(src);
}

PurchaseDownloadIssues::PurchaseDownloadIssues(PurchaseDownloadIssues&& src)
{
	std::copy(src.m_issues.begin(), src.m_issues.end(), std::back_inserter(m_issues));
	src.m_issues.clear();
}

PurchaseDownloadIssues& PurchaseDownloadIssues::operator=(const PurchaseDownloadIssues& right)
{
	DeepDelete(m_issues);
	m_issues.clear();

	PtrEnumerator<IPurchaseDownloadIssue> enumIssues = right.EnumIssues();
	while (enumIssues.hasNext())
	{
		Add(*enumIssues);
		enumIssues.advance();
	}
	return *this;
}

PurchaseDownloadIssue* PurchaseDownloadIssues::Add(ItemId id)
{
	PurchaseDownloadIssue* addedIssue = new PurchaseDownloadIssue(id);
	m_issues.push_back(addedIssue);
	return addedIssue;
}

PurchaseDownloadIssue* PurchaseDownloadIssues::Add(const IPurchaseDownloadIssue* issue)
{
	PurchaseDownloadIssue* addedIssue = new PurchaseDownloadIssue(issue);
	m_issues.push_back(addedIssue);
	return addedIssue;
}

PtrEnumerator<IPurchaseDownloadIssue> PurchaseDownloadIssues::EnumIssues() const
{
	return PtrEnumerator<IPurchaseDownloadIssue>(m_issues);
}

count_t PurchaseDownloadIssues::GetCount() const
{
	return m_issues.size();
}