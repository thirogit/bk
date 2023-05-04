#include "stdafx.h"
#include "CowPresenceAgeRangePredicate.h"
#include <string/TextUtils.h>

CowPresenceAgeRangePredicate::CowPresenceAgeRangePredicate(const IntRange& weightRange, CalendarUnit unit) : 
	m_ageRange(weightRange), m_unit(unit)
{
	m_now = DateTime::now();
}

bool CowPresenceAgeRangePredicate::Test(CowPresenceView* view)
{
	DateTime birthDt = view->GetBirthDate();
	int ageInDays = birthDt.GetDiffrenceInDays(m_now);

	if (ageInDays < 0) return false;

	if (birthDt.IsNull()) return false;

	switch (m_unit)
	{
	case CalendarUnit_Day:
		return m_ageRange.IsWithin(ageInDays);
	case CalendarUnit_Week:
		return m_ageRange.IsWithin(ageInDays/7);
	case CalendarUnit_Month:
	{
		DateTime bithDt1stMonth = DateTime(birthDt.GetYear(), birthDt.GetMonth(), 1);
		int ageInMonths = 0;
		while (bithDt1stMonth <= m_now)
		{
			bithDt1stMonth = bithDt1stMonth.AddMonth(1);
			ageInMonths++;
		}
		ageInMonths--;

		return m_ageRange.IsWithin(ageInMonths);

	}

	}
	return false;
}


