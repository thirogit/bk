#include "stdafx.h"
#include "CowPresenceSoldToPredicate.h"
#include <data\IIdable.h>

CowPresenceSoldToPredicate::CowPresenceSoldToPredicate(uint32_t hentId) : m_hentId(hentId)
{
}

bool CowPresenceSoldToPredicate::Test(CowPresenceView* view)
{
	if (m_hentId == NULL_ID)
	{
		return view->GetSellToHent() == NULL;
	}
	else
	{
		if (view->GetSellToHent() == NULL) return false;
		return view->GetSellToHent()->GetHentId() == m_hentId;
	}
}


