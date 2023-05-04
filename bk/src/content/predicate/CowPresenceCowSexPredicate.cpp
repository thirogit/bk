#include "stdafx.h"
#include "CowPresenceCowSexPredicate.h"


CowPresenceCowSexPredicate::CowPresenceCowSexPredicate(const CowSex& sex) :
	m_sex(sex)
{
	
}

bool CowPresenceCowSexPredicate::Test(CowPresenceView* view)
{
	return view->GetSex() == m_sex;
}


