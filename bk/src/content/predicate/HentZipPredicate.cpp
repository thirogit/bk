#include "stdafx.h"
#include "HentZipPredicate.h"
#include <string/TextUtils.h>

HentZipPredicate::HentZipPredicate(std::wstring& search) : m_search(search)
{
}

bool HentZipPredicate::Test(HentView* view)
{
	return TextUtils::ContainsNoCase(view->GetZip(), m_search);
}


