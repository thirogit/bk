#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/DateRange.h>


class CowPresenceHerdExitDatePredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceHerdExitDatePredicate(const DateRange& dtRange);
	virtual bool Test(CowPresenceView* view);
private:
	DateRange m_dtRange;
};

