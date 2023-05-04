#include "stdafx.h"
#include "DocViewText.h"
#include "../text/MotiveString.h"
#include <string/IntString.h>

std::wstring DocViewText::Text(const DocView *pView,uint32_t memberId)
{
	std::wstring text;
   	switch(memberId)
	{
		case DocViewMember::DocNumber:
			text = pView->GetDocNo();
			break;
		case DocViewMember::DocDate:
			text = pView->GetDocDate().GetDayDate();
			break;		
		case DocViewMember::LoadDate:
			text = pView->GetLoadDate().GetDayDate();
			break;
		case DocViewMember::PlateNo:
			text = pView->GetPlateNo();
			break;
		case DocViewMember::CowCount:
			text = IntString(pView->GetCowCount());
			break;
		case DocViewMember::Extras:
			text = pView->GetExtras();				
			break;
		case DocViewMember::Agent:			
			text = pView->GetAgentCd();				
			break;
		case DocViewMember::Motive:
			text = MotiveString(pView->GetMotive()).ToString();
			break;
		case DocViewMember::StartTime:
			text = pView->GetLoadStartDtTm().GetTime(Time::HHcolonMI);
			break;
		case DocViewMember::EndTime:
			text = pView->GetLoadEndDtTm().GetTime(Time::HHcolonMI);
			break;
		default:
			ASSERT(0);
			break;
	}	
	return text;
}
