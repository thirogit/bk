#include "stdafx.h"
#include "CowPresenceOwnStockPredicate.h"


CowPresenceOwnStockPredicate::CowPresenceOwnStockPredicate(uint32_t stockId) : 
	m_stockId(stockId)
{
	
}

bool CowPresenceOwnStockPredicate::Test(CowPresenceView* view)
{
	return view->GetOwnStockId() == m_stockId;
}


