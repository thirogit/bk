#include "stdafx.h"
#include "HentPersonalNoPredicate.h"
#include <string/TextUtils.h>

HentPersonalNoPredicate::HentPersonalNoPredicate(std::wstring& search) : m_search(search)
{
}

bool HentPersonalNoPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetPESEL(), m_search);
}


