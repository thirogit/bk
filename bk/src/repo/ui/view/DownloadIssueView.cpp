#include "stdafx.h"
#include "DownloadIssueView.h"

DownloadIssueView::DownloadIssueView(ItemId id) : m_Id(id)
{
}

ItemId DownloadIssueView::GetIssueId() const
{
	return m_Id;
}

PurchaseDownloadIssueDomain DownloadIssueView::GetDomain() const
{
	return m_Domain;
}

const std::wstring& DownloadIssueView::GetMesssage() const
{
	return m_Message;
}
	
void DownloadIssueView::SetDomain(PurchaseDownloadIssueDomain domain)
{
	m_Domain = domain;
}

void DownloadIssueView::SetMessage(const std::wstring& message)
{
	m_Message = message;
}
		