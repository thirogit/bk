#include "stdafx.h"
#include "PurchaseDownloadIssueCategory.h"


PurchaseDownloadIssueCategory::PurchaseDownloadIssueCategory() : m_domain(PurchaseDownloadIssueDomain_None)
{
}

PurchaseDownloadIssueCategory::PurchaseDownloadIssueCategory(const PurchaseDownloadIssueCategory& src)
{
	operator=(src);
}

PurchaseDownloadIssueCategory& PurchaseDownloadIssueCategory::operator=(const PurchaseDownloadIssueCategory& right)
{
	m_domain = right.m_domain;
	m_discriminator = right.m_discriminator;
	return *this;
}

PurchaseDownloadIssueCategory::PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain domain, const std::wstring& discriminator)
{
	m_domain = domain;
	m_discriminator = discriminator;
}

bool PurchaseDownloadIssueCategory::IsSame(const PurchaseDownloadIssueCategory& key)
{
	return m_domain == key.m_domain && m_discriminator == key.m_discriminator;
}

PurchaseDownloadIssueDomain PurchaseDownloadIssueCategory::GetDomain() const
{
	return m_domain;
}

const std::wstring& PurchaseDownloadIssueCategory::GetDiscriminator() const
{
	return m_discriminator;
}

