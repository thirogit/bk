#pragma once

#include "HentViewPredicate.h"


class HentHentNoPredicate : public HentViewPredicate
{
public:
	HentHentNoPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};

