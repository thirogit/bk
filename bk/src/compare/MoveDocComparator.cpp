#include "stdafx.h"
#include "MoveDocComparator.h"
#include <compare/TypesCompare.h>
#include <assert.h>

using namespace TypesCompare;

COMPARE_RESULT MoveDocComparator::CompareBy(UINT dataMember, MoveDocView* DataLeft, MoveDocView* DataRight) const
{
	MoveDocView *left = DataLeft, *right = DataRight;		

	switch(dataMember)
	{
		default:			
			return m_comparator.CompareBy(dataMember, DataLeft, DataRight);
		case MoveDocViewMember::SrcHerd:
			return CompareLocaleString(left->GetSrcHerdCd(), right->GetSrcHerdCd());		
		case MoveDocViewMember::DstHerd:
			return CompareLocaleString(left->GetDstHerdCd(), right->GetDstHerdCd());
	}

	ASSERT(0);
	return COMPARE_LESS;
}
