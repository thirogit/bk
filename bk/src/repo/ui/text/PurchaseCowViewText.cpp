#include "stdafx.h"
#include "PurchaseCowViewText.h"
#include "../../../text/CowSexText.h"
#include "../../../uiconstants/PrecisionContstants.h"

std::wstring PurchaseCowViewText::Text(const PurchaseCowView *pView,uint32_t memberId)
{
	std::wstring text;
   
	switch(memberId)
	{	
	case PurchaseCowViewMember::CowNo:
		text = pView->GetCowNo().ToString();
		break;

	case PurchaseCowViewMember::PassportNo:
		text = pView->GetPassNo();
		break;
	
	case PurchaseCowViewMember::FirstOwnerNo:
		text = pView->GetFirstOwnerNo().ToString();		
		break;

	case PurchaseCowViewMember::BirthDate:
		text = pView->GetBirthDate().GetDayDate();
		break;

	case PurchaseCowViewMember::Weight:
		text = pView->GetWeight().ToString(WEIGHT_PREC);
		break;

	case PurchaseCowViewMember::Sex:
		text = CowSexText(pView->GetSex()).ToString();
		break;

	case PurchaseCowViewMember::StockCd:
		text = pView->GetStockCd();			
		break;

	case PurchaseCowViewMember::ClassCd:
		text = pView->GetClassCd();		
		break;		

	case PurchaseCowViewMember::Price:
		text = pView->GetPrice().ToString(PRICE_PREC);
		break;
	
	default:
		ASSERT(0);
		break;
	}
	return text;
}
