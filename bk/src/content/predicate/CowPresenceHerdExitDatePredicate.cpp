#include "stdafx.h"
#include "CowPresenceHerdExitDatePredicate.h"

CowPresenceHerdExitDatePredicate::CowPresenceHerdExitDatePredicate(const DateRange& dtRange) : m_dtRange(dtRange)
{
}

bool CowPresenceHerdExitDatePredicate::Test(CowPresenceView* view)
{
	DateTime dt = view->GetHerdOutDate();
	if (dt.IsNull()) return false;

	return m_dtRange.IsDateWithin(dt);
}


