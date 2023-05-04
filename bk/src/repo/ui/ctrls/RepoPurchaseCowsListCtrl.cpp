#include "stdafx.h"
#include "RepoPurchaseCowsListCtrl.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"
#include "../view/PurchaseCowView.h"

CRepoPurchaseCowsListCtrl::CRepoPurchaseCowsListCtrl() 
{	
	ListHeaderDescriptor cowListHeader;
	cowListHeader(PurchaseCowViewMember::CowNo, StringLoader(IDS_HEADER_COWNO))
		(PurchaseCowViewMember::PassportNo, StringLoader(IDS_HEADER_PASSNO))
		(PurchaseCowViewMember::FirstOwnerNo, StringLoader(IDS_HEADER_FSTOWNER))
		(PurchaseCowViewMember::BirthDate, StringLoader(IDS_HEADER_BIRTHDATE))
		(PurchaseCowViewMember::Weight, StringLoader(IDS_HEADER_WEIGHT))
		(PurchaseCowViewMember::Sex, StringLoader(IDS_HEADER_SEX))
		(PurchaseCowViewMember::StockCd, StringLoader(IDS_HEADER_STOCK))
		(PurchaseCowViewMember::ClassCd, StringLoader(IDS_HEADER_CLASS))
		(PurchaseCowViewMember::Price, StringLoader(IDS_HEADER_NETPRICE));
		
	SetHeaderDescriptor(cowListHeader);
			
}
