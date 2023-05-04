#include "stdafx.h"
#include "HentStreetPredicate.h"
#include <string/TextUtils.h>

HentStreetPredicate::HentStreetPredicate(std::wstring& search) : m_search(search)
{
}

bool HentStreetPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetStreet(), m_search);
}


