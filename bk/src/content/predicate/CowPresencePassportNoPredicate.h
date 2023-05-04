#pragma once

#include "CowPresenceViewPredicate.h"

class CowPresencePassportNoPredicate : public CowPresenceViewPredicate
{
public:
	CowPresencePassportNoPredicate(std::wstring& search);
	virtual bool Test(CowPresenceView* view);
private:
	std::wstring m_search;
};

