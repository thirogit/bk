#pragma once

#include "HentViewPredicate.h"

class HentStreetPredicate : public HentViewPredicate
{
public:
	HentStreetPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};

