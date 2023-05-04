#include "stdafx.h"
#include "CowPresenceOwnClassPredicate.h"


CowPresenceOwnClassPredicate::CowPresenceOwnClassPredicate(uint32_t classId) :
	m_classId(classId)
{
	
}

bool CowPresenceOwnClassPredicate::Test(CowPresenceView* view)
{
	return view->GetOwnClassId() == m_classId;
}


