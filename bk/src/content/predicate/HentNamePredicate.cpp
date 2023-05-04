#include "stdafx.h"
#include "HentNamePredicate.h"
#include <string/TextUtils.h>

HentNamePredicate::HentNamePredicate(std::wstring& search) : m_search(search)
{
}

bool HentNamePredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetName(), m_search);
}


