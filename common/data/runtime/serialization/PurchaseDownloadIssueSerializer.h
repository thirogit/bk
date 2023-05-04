#pragma once

#include <SF/Archive.hpp>
#include "../PurchaseDownloadIssue.h"

class PurchaseDownloadIssueSerializer
{
public:
	PurchaseDownloadIssueSerializer(PurchaseDownloadIssue* issue);
	void serialize(SF::Archive &archive);
protected:
	ItemId& Id();
	std::wstring& Error();
	PurchaseDownloadIssueCategory& Category();
	PurchaseObjPath& Path();
	uint32_t& Solutions();
private:
	PurchaseDownloadIssue* m_issue;
};
