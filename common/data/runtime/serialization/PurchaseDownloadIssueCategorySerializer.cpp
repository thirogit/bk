#include "stdafx.h"
#include "PurchaseDownloadIssueCategorySerializer.h"
#include <SF/string.hpp>

PurchaseDownloadIssueCategorySerializer::PurchaseDownloadIssueCategorySerializer(PurchaseDownloadIssueCategory* category) : m_category(category)
{
}

void PurchaseDownloadIssueCategorySerializer::serialize(SF::Archive &archive)
{
	archive & Domain() & Discriminator();
}

PurchaseDownloadIssueDomain& PurchaseDownloadIssueCategorySerializer::Domain()
{
	return m_category->m_domain;
}

std::wstring& PurchaseDownloadIssueCategorySerializer::Discriminator()
{
	return m_category->m_discriminator;
}

