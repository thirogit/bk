#include "stdafx.h"
#include "DocItemViewText.h"
#include "CowSexText.h"
#include "../uiconstants/PrecisionContstants.h"

std::wstring DocItemViewText::Text(const DocItemView *pView,uint32_t memberId)
{
	std::wstring text;
   	switch(memberId)
	{
		case DocItemViewMember::DocItemView_CowNo:
			text = pView->GetCowNo().ToString();
			break;
		case DocItemViewMember::DocItemView_PassNo:
			text = pView->GetPassNo();
			break;
		case DocItemViewMember::DocItemView_FirstOwner:
			text = pView->GetFstOwnerAlias();
			break;
		case DocItemViewMember::DocItemView_BirthDate:
			text = pView->GetBirthDate().GetDayDate();
			break;
		case DocItemViewMember::DocItemView_Weight:
			text = pView->GetWeight().Object().ToString(WEIGHT_PREC);
			break;
		case DocItemViewMember::DocItemView_Sex:
			text = CowSexText(pView->GetSex()).ToString();
			break;
		case DocItemViewMember::DocItemView_Stock:
			text = pView->GetStock().Object();
			break;
		case DocItemViewMember::DocItemView_Class:
			text = pView->GetClass().Object();
			break;
		case DocItemViewMember::DocItemView_Group:
			text = pView->GetGroup().Object().ToString();
			break;
		
		default:
			ASSERT(0);
			break;
	}	
	return text;
}
