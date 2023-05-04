#include "stdafx.h"
#include "DocComparator.h"
#include <compare/TypesCompare.h>
#include <assert.h>
#include "../text/MotiveString.h"

using namespace TypesCompare;

COMPARE_RESULT DocComparator::CompareBy(UINT dataMember, DocView* DataLeft, DocView* DataRight) const
{
	DocView *left = DataLeft, *right = DataRight;		

	switch(dataMember)
	{
		default:
			ASSERT(0);

		case DocViewMember::DocNumber:			
			return CompareLocaleString(left->GetDocNo(), right->GetDocNo());
		case DocViewMember::DocDate:
			return CompareDateTime(left->GetDocDate() , right->GetDocDate());
		case DocViewMember::LoadDate:
			return CompareDateTime(left->GetLoadDate() , right->GetLoadDate());		
		case DocViewMember::PlateNo:
			return CompareLocaleString(left->GetPlateNo(), right->GetPlateNo());
		case DocViewMember::CowCount:			
			return CompareInt(left->GetCowCount(),right->GetCowCount());			
		case DocViewMember::Extras:
			return CompareLocaleString(left->GetExtras(), right->GetExtras());
		case DocViewMember::Agent:
			return CompareLocaleString(left->GetAgentCd(), right->GetAgentCd()); 
		case DocViewMember::Motive:
			return CompareLocaleString(MotiveString(left->GetMotive()).ToString(), MotiveString(right->GetMotive()).ToString());
		case DocViewMember::StartTime:
			return CompareNullable(left->GetLoadStartDtTm(), right->GetLoadStartDtTm());
		case DocViewMember::EndTime:
			return CompareNullable(left->GetLoadEndDtTm(), right->GetLoadEndDtTm());
	}

	ASSERT(0);
	return COMPARE_LESS;
}
