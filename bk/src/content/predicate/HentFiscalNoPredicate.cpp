#include "stdafx.h"
#include "HentFiscalNoPredicate.h"
#include <string/TextUtils.h>

HentFiscalNoPredicate::HentFiscalNoPredicate(std::wstring& search) : m_search(search)
{
}

bool HentFiscalNoPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetFiscalNo(), m_search);
}


