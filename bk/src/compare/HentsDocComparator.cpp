#include "stdafx.h"
#include "HentsDocComparator.h"
#include <compare/TypesCompare.h>
#include <assert.h>

using namespace TypesCompare;

COMPARE_RESULT HentsDocComparator::CompareBy(UINT dataMember, HentsDocView* DataLeft, HentsDocView* DataRight) const
{
	HentsDocView *left = DataLeft, *right = DataRight;

	switch(dataMember)
	{
		default:			
			return m_comparator.CompareBy(dataMember, DataLeft, DataRight);
		case HentsDocViewMember::Herd:
			return CompareLocaleString(left->GetHerdCd(), right->GetHerdCd());		
		case HentsDocViewMember::Hent:
			return CompareLocaleString(left->GetHentAlias(), right->GetHentAlias());
		case HentsDocViewMember::Invoice:
			return CompareLocaleString(left->GetInvoiceNo(), right->GetInvoiceNo());
	}

	ASSERT(0);
	return COMPARE_LESS;
}
