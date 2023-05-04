#include "stdafx.h"
#include "CowPresenceCowNoPredicate.h"
#include <string/TextUtils.h>

CowPresenceCowNoPredicate::CowPresenceCowNoPredicate(std::wstring& search) : m_search(search)
{
}

bool CowPresenceCowNoPredicate::Test(CowPresenceView* view)
{
	return TextUtils::ContainsNoCase(view->GetCowNo().ToString(), m_search);
}


