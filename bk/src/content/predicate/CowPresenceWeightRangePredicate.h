#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/DecimalRange.h>


class CowPresenceWeightRangePredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceWeightRangePredicate(const DecimalRange& weightRange);
	virtual bool Test(CowPresenceView* view);
private:
	DecimalRange m_weightRange;
};

