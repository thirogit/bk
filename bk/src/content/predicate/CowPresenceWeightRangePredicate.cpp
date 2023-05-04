#include "stdafx.h"
#include "CowPresenceWeightRangePredicate.h"
#include <string/TextUtils.h>

CowPresenceWeightRangePredicate::CowPresenceWeightRangePredicate(const DecimalRange& weightRange) : m_weightRange(weightRange)
{
}

bool CowPresenceWeightRangePredicate::Test(CowPresenceView* view)
{
	return m_weightRange.IsWithin(view->GetWeight());
}


