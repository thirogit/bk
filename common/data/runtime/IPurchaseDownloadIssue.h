#pragma once

#include "IItem.h"
#include "../repo/PurchaseObjPath.h"
#include "PurchaseDownloadIssueCategory.h"
#include "PurchaseDownloadIssueDomain.h"


class IPurchaseDownloadIssue : public IItem
{
public:	
	virtual const PurchaseObjPath& GetPath() const = 0;
	virtual const std::wstring& GetError() const = 0;
	virtual uint32_t GetSolutions() const = 0;
	virtual const PurchaseDownloadIssueCategory& GetCategory() const = 0;
};


