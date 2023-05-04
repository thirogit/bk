#pragma once

#include "CowPresenceViewPredicate.h"

class CowPresenceOwnStockPredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceOwnStockPredicate(uint32_t stockId);
	virtual bool Test(CowPresenceView* view);
private:
	uint32_t m_stockId;
};

