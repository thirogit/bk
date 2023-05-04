#include "stdafx.h"
#include "PurchaseDownloadIssue.h"
#include "SolutionType.h"

PurchaseDownloadIssue::PurchaseDownloadIssue(ItemId id) : m_issueId(id), m_Solutions(SolutionType_None)
{
}

PurchaseDownloadIssue::PurchaseDownloadIssue(const IPurchaseDownloadIssue* src)
{
	m_issueId = src->GetId();
	CopyFrom(src);	
}

PurchaseDownloadIssue::PurchaseDownloadIssue(const PurchaseDownloadIssue& src)
{
	m_issueId = src.GetId();
	CopyFrom(&src);
}

void PurchaseDownloadIssue::CopyFrom(const IPurchaseDownloadIssue* src)
{
	m_path = src->GetPath();
	m_category = src->GetCategory();
	m_errorTokens = src->GetError();
	m_Solutions = src->GetSolutions();
}

ItemId PurchaseDownloadIssue::GetId() const
{
	return m_issueId;
}

void PurchaseDownloadIssue::SetPath(const PurchaseObjPath& path)
{
	m_path = path;
}

void PurchaseDownloadIssue::SetCategory(const PurchaseDownloadIssueCategory& category)
{
	m_category = category;
}

void PurchaseDownloadIssue::SetError(const std::wstring& error)
{
	m_errorTokens = error;
}

const PurchaseObjPath& PurchaseDownloadIssue::GetPath() const
{
	return m_path;
}

const PurchaseDownloadIssueCategory& PurchaseDownloadIssue::GetCategory() const
{
	return m_category;
}

const std::wstring& PurchaseDownloadIssue::GetError() const
{
	return m_errorTokens;
}

uint32_t PurchaseDownloadIssue::GetSolutions() const
{
	return m_Solutions;
}

void PurchaseDownloadIssue::SetSolutions(uint32_t solutions)
{
	m_Solutions = solutions;
}



