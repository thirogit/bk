#include "stdafx.h"
#include "HentHentTypePredicate.h"


HentHentTypePredicate::HentHentTypePredicate(HentType search) : m_search(search)
{
}

bool HentHentTypePredicate::Test(HentView* view)
{
	return view->GetHentType() == m_search;
}


