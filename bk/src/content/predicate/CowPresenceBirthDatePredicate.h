#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/DateRange.h>


class CowPresenceBirthDatePredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceBirthDatePredicate(const DateRange& dtRange);
	virtual bool Test(CowPresenceView* view);
private:
	DateRange m_dtRange;
};

