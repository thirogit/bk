#pragma once

#include "PurchaseDownloadIssue.h"
#include <vector>
#include <arrays/Enumerator.h>
#include "../../CommonTypes.h"

class PurchaseDownloadIssuesSerializer;

class PurchaseDownloadIssues
{
public:
	PurchaseDownloadIssues();
	~PurchaseDownloadIssues();
	PurchaseDownloadIssues(const PurchaseDownloadIssues& src);
	PurchaseDownloadIssues(PurchaseDownloadIssues&& src);
	PurchaseDownloadIssues& operator=(const PurchaseDownloadIssues& right);
	PurchaseDownloadIssues& operator=(PurchaseDownloadIssues&& right);
	count_t GetCount() const;

	PurchaseDownloadIssue* Add(ItemId id);
	PurchaseDownloadIssue* Add(const IPurchaseDownloadIssue* issue);
	PtrEnumerator<IPurchaseDownloadIssue> EnumIssues() const;
		
private:
	std::vector<PurchaseDownloadIssue*> m_issues;

	friend class PurchaseDownloadIssuesSerializer;
};


