#include "stdafx.h"
#include "PurchaseIssuesListCtrl.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"
#include "../view/DownloadIssueView.h"

CPurchaseIssuesListCtrl::CPurchaseIssuesListCtrl() 
{	
	ListHeaderDescriptor cowListHeader;
	cowListHeader(DownloadIssueViewMember::Message, StringLoader(IDS_HEADER_ISSUE))
		(DownloadIssueViewMember::Domain, StringLoader(IDS_HEADER_ISSUEDOMAIN))
		;
		
		
	SetHeaderDescriptor(cowListHeader);
			
}
