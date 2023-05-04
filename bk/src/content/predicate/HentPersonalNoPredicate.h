#pragma once

#include "HentViewPredicate.h"

class HentPersonalNoPredicate : public HentViewPredicate
{
public:
	HentPersonalNoPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};


