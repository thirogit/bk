#include "stdafx.h"
#include "CowViewText.h"
#include "../text/CowSexText.h"
#include "../uiconstants/PrecisionContstants.h"

std::wstring CowViewText::Text(const CowView *pView,uint32_t memberId)
{
	std::wstring text;
   
	switch(memberId)
	{
	
	case CowViewMembers::CowView_CowNo:
		text = pView->GetCowNo().ToString();
		break;

	case CowViewMembers::CowView_PassNo:
		text = pView->GetPassNo();
		break;
	
	case CowViewMembers::CowView_FirstOwner:
		text = pView->GetFirstOwnerAlias();		
		break;

	case CowViewMembers::CowView_BirthDate:
		text = pView->GetBirthDate().GetDayDate();
		break;

	case CowViewMembers::CowView_Weight:
		text = pView->GetWeight().ToString(WEIGHT_PREC);
		break;

	case CowViewMembers::CowView_Sex:
		text = CowSexText(pView->GetSex()).ToString();
		break;

	case CowViewMembers::CowView_Stock:
		text = pView->GetStockCd();			
		break;

	case CowViewMembers::CowView_Class:
		text = pView->GetClassCd();		
		break;
		
	case CowViewMembers::CowView_TermBuyClass:		
		text = pView->GetTermBuyClassCd();
		break;

	case CowViewMembers::CowView_TermBuyWeight:
		text = pView->GetTermBuyWeight().ToString(WEIGHT_PREC);		
		break;

	case CowViewMembers::CowView_TermBuyPrice:
		text = pView->GetTermBuyPrice().ToString(PRICE_PREC);				
		break;

	case CowViewMembers::CowView_TermSellClass:
		text = pView->GetTermSellClassCd();
		break;

	case CowViewMembers::CowView_TermSellWeight:
		text = pView->GetTermSellWeight().ToString(WEIGHT_PREC);
		break;

	case CowViewMembers::CowView_TermSellPrice:
		text = pView->GetTermSellPrice().ToString(WEIGHT_PREC);								
		break;

	default:
		ASSERT(0);
		break;
	}
	return text;
}
