#include "stdafx.h"
#include "HentCityPredicate.h"
#include <string/TextUtils.h>

HentCityPredicate::HentCityPredicate(std::wstring& search) : m_search(search)
{
}

bool HentCityPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetCity(), m_search);
}


