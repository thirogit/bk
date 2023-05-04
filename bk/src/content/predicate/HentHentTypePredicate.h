#pragma once

#include "HentViewPredicate.h"


class HentHentTypePredicate : public HentViewPredicate
{
public:
	HentHentTypePredicate(HentType search);
	virtual bool Test(HentView* view);
private:
	HentType m_search;
};
