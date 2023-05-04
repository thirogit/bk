#include "stdafx.h"
#include "CowPresenceBirthDatePredicate.h"

CowPresenceBirthDatePredicate::CowPresenceBirthDatePredicate(const DateRange& dtRange) : m_dtRange(dtRange)
{
}

bool CowPresenceBirthDatePredicate::Test(CowPresenceView* view)
{
	DateTime birthDt = view->GetBirthDate();
	if (birthDt.IsNull()) return false;

	return m_dtRange.IsDateWithin(birthDt);
}


