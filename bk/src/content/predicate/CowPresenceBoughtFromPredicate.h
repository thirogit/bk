#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/DateRange.h>


class CowPresenceBoughtFromPredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceBoughtFromPredicate(uint32_t hentId);
	virtual bool Test(CowPresenceView* view);
private:
	uint32_t m_hentId;
};

