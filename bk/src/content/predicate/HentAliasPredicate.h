#pragma once
#include "HentViewPredicate.h"

class HentAliasPredicate : public HentViewPredicate
{
public:
	HentAliasPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};


