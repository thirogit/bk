#include "stdafx.h"
#include "SeasonText.h"
#include "..\compare\SeasonComparator.h"

std::wstring SeasonText::Text(const ISeason *pSeason,uint32_t memberId)
{
	std::wstring s;
   	switch(memberId)
	{
		case SeasonMember_Name:
			s = pSeason->GetSeasonName();
			break;
		case SeasonMember_StartDt:
			s = pSeason->GetStartDate().GetDayDate();
			break;
		case SeasonMember_EndDt:
			s = pSeason->GetEndDate().GetDayDate();
			break;		
		default:
			_ASSERT(0);
			break;
	}	
	return s;
}
