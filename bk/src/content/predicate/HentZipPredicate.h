#pragma once

#include "HentViewPredicate.h"

class HentZipPredicate : public HentViewPredicate
{
public:
	HentZipPredicate(std::wstring& search);
	virtual bool Test(HentView* view);
private:
	std::wstring m_search;
};


