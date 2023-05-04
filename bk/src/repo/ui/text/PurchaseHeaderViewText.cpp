#include "stdafx.h"
#include "PurchaseHeaderViewText.h"
#include <string\TextUtils.h>
#include "../../../text/YesNoText.h"

std::wstring PurchaseHeaderViewText::Text(const PurchaseHeaderView *pView,uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
	case PurchaseHeaderViewMember::PurchaseHeaderViewMember_Herd:
		text = TextUtils::Format(L"%03d", pView->GetHerd());
		break;
	case PurchaseHeaderViewMember::PurchaseHeaderViewMember_Agent:
		text = pView->GetAgent();
		break;
	case PurchaseHeaderViewMember::PurchaseHeaderViewMember_StartDt:
		text = pView->GetStartDt().GetDateInFmt(DateTime::YYYYdashMMdashDD_HHcolonMI);
		break;
	case PurchaseHeaderViewMember::PurchaseHeaderViewMember_CowAmount:
		text = TextUtils::Format(L"%d", pView->GetCowAmount());
		break;
	case PurchaseHeaderViewMember::PurchaseHeaderViewMember_PlateNo:
		text = pView->GetPlateNo();
		break;
	case PurchaseHeaderViewMember::PurchaseHeaderViewMember_Downloaded:
		text = YesNoText(pView->GetDownloaded()).ToString();
		break;
	}	
	return text;
}
