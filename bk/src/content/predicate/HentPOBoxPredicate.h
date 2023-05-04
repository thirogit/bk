#pragma once

#include "HentViewPredicate.h"

class HentPOBoxPredicate : public HentViewPredicate
{
public:
	HentPOBoxPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};


