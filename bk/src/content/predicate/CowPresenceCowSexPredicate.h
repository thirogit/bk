#pragma once

#include "CowPresenceViewPredicate.h"

class CowPresenceCowSexPredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceCowSexPredicate(const CowSex& sex);
	virtual bool Test(CowPresenceView* view);
private:
	CowSex m_sex;
};

