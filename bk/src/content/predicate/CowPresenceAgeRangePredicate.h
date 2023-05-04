#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/IntRange.h>
#include <types/CalendarUnit.h>


class CowPresenceAgeRangePredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceAgeRangePredicate(const IntRange& weightRange, CalendarUnit unit);
	virtual bool Test(CowPresenceView* view);
private:
	IntRange m_ageRange;
	CalendarUnit m_unit;
	DateTime m_now;
};

