#include "stdafx.h"
#include "HentAliasPredicate.h"
#include <string/TextUtils.h>

HentAliasPredicate::HentAliasPredicate(std::wstring& search) : m_search(search)
{
}

bool HentAliasPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetAlias(), m_search);
}


