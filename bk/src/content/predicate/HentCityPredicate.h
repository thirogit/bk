#pragma once

#include "HentViewPredicate.h"


class HentCityPredicate : public HentViewPredicate
{
public:
	HentCityPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};


