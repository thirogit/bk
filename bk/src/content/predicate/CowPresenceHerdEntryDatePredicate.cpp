#include "stdafx.h"
#include "CowPresenceHerdEntryDatePredicate.h"

CowPresenceHerdEntryDatePredicate::CowPresenceHerdEntryDatePredicate(const DateRange& dtRange) : m_dtRange(dtRange)
{
}

bool CowPresenceHerdEntryDatePredicate::Test(CowPresenceView* view)
{
	DateTime dt = view->GetHerdInDate();
	if (dt.IsNull()) return false;

	return m_dtRange.IsDateWithin(dt);
}


