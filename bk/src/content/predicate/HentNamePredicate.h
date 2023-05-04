#pragma once

#include "HentViewPredicate.h"


class HentNamePredicate : public HentViewPredicate
{
public:
	HentNamePredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};


