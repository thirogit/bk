#pragma once

#include <SF/Archive.hpp>
#include "../PurchaseDownloadIssues.h"

class PurchaseDownloadIssuesSerializer
{
public:
	PurchaseDownloadIssuesSerializer(PurchaseDownloadIssues* issues);
	void serialize(SF::Archive &archive);
private:
	PurchaseDownloadIssues* m_issues;
};
