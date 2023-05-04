#include "stdafx.h"
#include "CowPresencePassportNoPredicate.h"
#include <string/TextUtils.h>

CowPresencePassportNoPredicate::CowPresencePassportNoPredicate(std::wstring& search) : m_search(search)
{
}

bool CowPresencePassportNoPredicate::Test(CowPresenceView* view)
{
	return TextUtils::ContainsNoCase(view->GetPassNo(), m_search);
}


