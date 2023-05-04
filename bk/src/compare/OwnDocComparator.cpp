#include "stdafx.h"
#include "OwnDocComparator.h"
#include <compare/TypesCompare.h>
#include <assert.h>

using namespace TypesCompare;

COMPARE_RESULT OwnDocComparator::CompareBy(UINT dataMember, OwnDocView* DataLeft, OwnDocView* DataRight) const
{
	OwnDocView *left = DataLeft, *right = DataRight;		

	switch(dataMember)
	{
		default:			
			return m_comparator.CompareBy(dataMember, DataLeft, DataRight);
		case OwnDocViewMember::Herd:
			return CompareLocaleString(left->GetHerdCd(), right->GetHerdCd());		
	}

	ASSERT(0);
	return COMPARE_LESS;
}
