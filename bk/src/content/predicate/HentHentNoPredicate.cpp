#include "stdafx.h"
#include "HentHentNoPredicate.h"
#include <string/TextUtils.h>

HentHentNoPredicate::HentHentNoPredicate(std::wstring& search) : m_search(search)
{
}

bool HentHentNoPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetHentNo().ToString(), m_search);
}


