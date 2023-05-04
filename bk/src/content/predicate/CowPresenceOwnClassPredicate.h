#pragma once

#include "CowPresenceViewPredicate.h"

class CowPresenceOwnClassPredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceOwnClassPredicate(uint32_t classId);
	virtual bool Test(CowPresenceView* view);
private:
	uint32_t m_classId;
};

