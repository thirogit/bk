#include "stdafx.h"
#include "RepoPurchaseListCtrl.h"
#include "../view/PurchaseHeaderView.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CRepoPurchaseListCtrl, CContentListCtrl)
	
END_MESSAGE_MAP()

CRepoPurchaseListCtrl::CRepoPurchaseListCtrl() 
{	
	ListHeaderDescriptor header;
	header(PurchaseHeaderViewMember::PurchaseHeaderViewMember_Herd, StringLoader(IDS_HEADER_HERD))
		(PurchaseHeaderViewMember::PurchaseHeaderViewMember_StartDt, StringLoader(IDS_HEADER_STARTDTM))
		(PurchaseHeaderViewMember::PurchaseHeaderViewMember_Agent, StringLoader(IDS_HEADER_AGENT))
		(PurchaseHeaderViewMember::PurchaseHeaderViewMember_CowAmount, StringLoader(IDS_HEADER_COWCOUNT))
		(PurchaseHeaderViewMember::PurchaseHeaderViewMember_PlateNo, StringLoader(IDS_HEADER_PLATENO))
		(PurchaseHeaderViewMember::PurchaseHeaderViewMember_Downloaded, StringLoader(IDS_HEADER_DOWNLOADED));

	SetHeaderDescriptor(header);
}
