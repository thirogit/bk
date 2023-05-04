#include "stdafx.h"
#include "PurchaseDownloadIssuesSerializer.h"
#include "PurchaseDownloadIssueSerializer.h"
#include <boost/foreach.hpp>

PurchaseDownloadIssuesSerializer::PurchaseDownloadIssuesSerializer(PurchaseDownloadIssues* issues) : m_issues(issues)
{
}

void PurchaseDownloadIssuesSerializer::serialize(SF::Archive &archive)
{
	count_t issueCount;
	if (archive.isWrite())
	{
		issueCount = m_issues->GetCount();
		archive & issueCount;

		BOOST_FOREACH(PurchaseDownloadIssue* issue, m_issues->m_issues)
		{
			PurchaseDownloadIssueSerializer(issue).serialize(archive);
		}
	}
	else
	{
		PurchaseDownloadIssue* issue = new PurchaseDownloadIssue(ItemId());
		archive & issueCount;
		for (int i = 0; i < issueCount; i++)
		{
			PurchaseDownloadIssueSerializer(issue).serialize(archive);
			m_issues->Add(issue);			
		}
		delete issue;
	}
}
