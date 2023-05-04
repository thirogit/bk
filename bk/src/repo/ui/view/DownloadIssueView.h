#ifndef __DOWNLOADISSUEVIEW_H__
#define __DOWNLOADISSUEVIEW_H__

#include <string>
#include <stdint.h>
#include <types\DateTime.h>
#include <data\runtime\PurchaseDownloadIssue.h>
#include <data\runtime\ItemId.h>

struct DownloadIssueViewMember
{
	enum
	{		
		Domain,
		Message,		
	};
};

class DownloadIssueView 
{
public:
	DownloadIssueView(ItemId id);	
	ItemId GetIssueId() const;	
	
	PurchaseDownloadIssueDomain GetDomain() const;
	const std::wstring& GetMesssage() const;
	
	void SetDomain(PurchaseDownloadIssueDomain domain);
	void SetMessage(const std::wstring& message);
		
private:
	ItemId m_Id;
	PurchaseDownloadIssueDomain m_Domain;
	std::wstring m_Message;	
};

#endif