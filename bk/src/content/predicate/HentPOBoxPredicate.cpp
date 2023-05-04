#include "stdafx.h"
#include "HentPOBoxPredicate.h"
#include <string/TextUtils.h>

HentPOBoxPredicate::HentPOBoxPredicate(std::wstring& search) : m_search(search)
{
}

bool HentPOBoxPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetPOBox(), m_search);
}


