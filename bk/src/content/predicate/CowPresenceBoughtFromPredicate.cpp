#include "stdafx.h"
#include "CowPresenceBoughtFromPredicate.h"
#include <data\IIdable.h>

CowPresenceBoughtFromPredicate::CowPresenceBoughtFromPredicate(uint32_t hentId) : m_hentId(hentId)
{
}

bool CowPresenceBoughtFromPredicate::Test(CowPresenceView* view)
{
	if (m_hentId == NULL_ID)
	{
		return view->GetBuyFromHent() == NULL;
	}
	else
	{
		if (view->GetBuyFromHent() == NULL) return false;
		return view->GetBuyFromHent()->GetHentId() == m_hentId;
	}
}


