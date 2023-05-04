#pragma once

#include "HentViewPredicate.h"

class HentFiscalNoPredicate : public HentViewPredicate
{
public:
	HentFiscalNoPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};
