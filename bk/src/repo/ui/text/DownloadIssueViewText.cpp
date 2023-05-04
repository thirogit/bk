#include "stdafx.h"
#include "DownloadIssueViewText.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"

std::wstring DownloadIssueViewText::Text(const DownloadIssueView *pView, uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
	case DownloadIssueViewMember::Domain:
		text = DomainToText(pView->GetDomain());
		break;
	case DownloadIssueViewMember::Message:
		text = pView->GetMesssage();
		break;	
	}	
	return text;
}

std::wstring DownloadIssueViewText::DomainToText(PurchaseDownloadIssueDomain domain)
{
	std::wstring text;
	switch (domain)
	{
	case PurchaseDownloadIssueDomain_Herd:
		text = StringLoader(IDS_ISSUEDOMAIN_HERD);
		break;
	case PurchaseDownloadIssueDomain_Agent:
		text = StringLoader(IDS_ISSUEDOMAIN_AGENT);
		break;
	case PurchaseDownloadIssueDomain_Hent:
		text = StringLoader(IDS_ISSUEDOMAIN_HENT);
		break;
	case PurchaseDownloadIssueDomain_Class:
		text = StringLoader(IDS_ISSUEDOMAIN_CLASS);
		break;
	case PurchaseDownloadIssueDomain_Stock:
		text = StringLoader(IDS_ISSUEDOMAIN_STOCK);
		break;
	case PurchaseDownloadIssueDomain_Cow:
		text = StringLoader(IDS_ISSUEDOMAIN_COW);
		break;
	}
	return text;
}