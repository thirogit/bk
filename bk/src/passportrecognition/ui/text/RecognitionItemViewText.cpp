#include "stdafx.h"
#include "RecognitionItemViewText.h"
#include "../../../text/CowSexText.h"
#include "../../../uiconstants/PrecisionContstants.h"

std::wstring RecognitionItemViewText::Text(const RecognitionItemView *pView, uint32_t memberId)
{
	std::wstring text;
   
	switch(memberId)
	{
	
	case RecognitionItemViewMembers::RecognitionItemView_CowNo:
		text = pView->GetCowNo().ToString();
		break;

	case RecognitionItemViewMembers::RecognitionItemView_PassNo:
		text = pView->GetPassNo();
		break;
	
	case RecognitionItemViewMembers::RecognitionItemView_FirstOwner:
		text = pView->GetFirstOwnerAlias();		
		break;

	case RecognitionItemViewMembers::RecognitionItemView_BirthDate:
		text = pView->GetBirthDate().GetDayDate();
		break;

	case RecognitionItemViewMembers::RecognitionItemView_Weight:
		text = pView->GetWeight().ToString(WEIGHT_PREC);
		break;

	case RecognitionItemViewMembers::RecognitionItemView_Sex:
		text = CowSexText(pView->GetSex()).ToString();
		break;

	case RecognitionItemViewMembers::RecognitionItemView_Stock:
		text = pView->GetStockCd();			
		break;

	case RecognitionItemViewMembers::RecognitionItemView_Class:
		text = pView->GetClassCd();	
		break;	

	default:
		ASSERT(0);
		break;
	}
	return text;
}
