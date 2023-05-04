#pragma once

#include "PurchaseDownloadIssueDomain.h"
#include <string>
#include "ItemId.h"

class PurchaseDownloadIssueCategorySerializer;

class PurchaseDownloadIssueCategory
{
public:
	PurchaseDownloadIssueCategory();
	PurchaseDownloadIssueCategory(const PurchaseDownloadIssueCategory& src);
	PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain domain,const std::wstring& discriminator);
	PurchaseDownloadIssueCategory& operator=(const PurchaseDownloadIssueCategory& right);
	bool IsSame(const PurchaseDownloadIssueCategory& key);
	
	PurchaseDownloadIssueDomain GetDomain() const;
	const std::wstring& GetDiscriminator() const;

private:
	PurchaseDownloadIssueDomain m_domain;
	std::wstring m_discriminator;
		
	friend class PurchaseDownloadIssueCategorySerializer;
};
