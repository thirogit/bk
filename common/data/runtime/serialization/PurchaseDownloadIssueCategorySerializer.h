#pragma once

#include <SF/Archive.hpp>
#include "../PurchaseDownloadIssueCategory.h"

class PurchaseDownloadIssueCategorySerializer
{
public:
	PurchaseDownloadIssueCategorySerializer(PurchaseDownloadIssueCategory* category);
	void serialize(SF::Archive &archive);
private:
	PurchaseDownloadIssueDomain& Domain();
	std::wstring& Discriminator();	
private:
	PurchaseDownloadIssueCategory* m_category;
	
};
