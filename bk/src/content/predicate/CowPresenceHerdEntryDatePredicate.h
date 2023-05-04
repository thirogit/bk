#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/DateRange.h>


class CowPresenceHerdEntryDatePredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceHerdEntryDatePredicate(const DateRange& dtRange);
	virtual bool Test(CowPresenceView* view);
private:
	DateRange m_dtRange;
};

