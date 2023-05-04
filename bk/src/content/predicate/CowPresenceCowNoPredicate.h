#pragma once

#include "CowPresenceViewPredicate.h"


class CowPresenceCowNoPredicate : public CowPresenceViewPredicate
{
public:
	CowPresenceCowNoPredicate(std::wstring& search);
	virtual bool Test(CowPresenceView* view);
private:
	std::wstring m_search;
};

