#pragma once

#include "DownloadIssueView.h"
#include <data\runtime\IPurchaseDownloadIssue.h>
#include <map>

class DownloadIssueViewCreator 
{
public:
	DownloadIssueViewCreator();
	DownloadIssueView* CreateView(const IPurchaseDownloadIssue* pIssue);
	void RefreshView(DownloadIssueView* pView,const IPurchaseDownloadIssue* pIssue);
private:
	std::map<std::wstring,uint32_t> m_issueCodeToMsg;
};
