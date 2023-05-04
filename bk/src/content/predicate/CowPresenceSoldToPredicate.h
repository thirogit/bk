#pragma once

#include "CowPresenceViewPredicate.h"
#include <types/DateRange.h>


class CowPresenceSoldToPredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceSoldToPredicate(uint32_t hentId);
	virtual bool Test(CowPresenceView* view);
private:
	uint32_t m_hentId;
};

