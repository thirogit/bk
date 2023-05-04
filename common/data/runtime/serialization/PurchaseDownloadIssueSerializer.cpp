#include "stdafx.h"
#include "PurchaseDownloadIssueSerializer.h"
#include <SF/string.hpp>
#include "../../repo/serialization/RepoTypesSerialization.h"
#include "RuntimeTypesSerialization.h"

PurchaseDownloadIssueSerializer::PurchaseDownloadIssueSerializer(PurchaseDownloadIssue* issue) : m_issue(issue)
{
}

ItemId& PurchaseDownloadIssueSerializer::Id()
{
	return m_issue->m_issueId;
}

std::wstring& PurchaseDownloadIssueSerializer::Error()
{
	return m_issue->m_errorTokens;
}

PurchaseDownloadIssueCategory& PurchaseDownloadIssueSerializer::Category()
{
	return m_issue->m_category;
}

PurchaseObjPath& PurchaseDownloadIssueSerializer::Path()
{
	return m_issue->m_path;
}

uint32_t& PurchaseDownloadIssueSerializer::Solutions()
{
	return m_issue->m_Solutions;
}

void PurchaseDownloadIssueSerializer::serialize(SF::Archive &archive)
{
	archive & Id() & Category() & Path() & Error() & Solutions();
}
