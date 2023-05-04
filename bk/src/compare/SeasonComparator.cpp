#include "stdafx.h"
#include "SeasonComparator.h"
#include <compare/TypesCompare.h>

using namespace TypesCompare;

COMPARE_RESULT SeasonComparator::CompareBy(uint32_t memberId,ISeason* DataLeft,ISeason* DataRight) const
{
	ISeason *left = DataLeft,*right = DataRight;
	
	switch(memberId)
	{		
		case SeasonMember_Name:
			return CompareLocaleString(left->GetSeasonName(), right->GetSeasonName());
		case SeasonMember_StartDt:
			return CompareDateTime(left->GetStartDate(),right->GetStartDate());
		case SeasonMember_EndDt:
			return CompareDateTime(left->GetEndDate(),right->GetEndDate());
		default:
			_ASSERT(0);
	}

	return COMPARE_LESS;
}
