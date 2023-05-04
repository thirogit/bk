#ifndef __DOWNLOADISSUEVIEWTEXT_H__
#define __DOWNLOADISSUEVIEWTEXT_H__

#include "../view/DownloadIssueView.h"

class DownloadIssueViewText 
{
public:
	virtual std::wstring Text(const DownloadIssueView *pView,uint32_t memberId);
private:
	std::wstring DomainToText(PurchaseDownloadIssueDomain domain);
};

#endif