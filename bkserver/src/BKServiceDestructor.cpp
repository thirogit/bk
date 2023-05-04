#include "stdafx.h"
#include "BKServiceDestructor.h"
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include "FarmSessionFinalizer.h"

BKServiceDestructor::BKServiceDestructor(BKService* pService) : m_pService(pService)
{
}

void BKServiceDestructor::Destroy()
{
	
	BOOST_FOREACH(FarmSession* farmSession,m_pService->m_FarmSessions | boost::adaptors::map_values)
	{
		FarmSessionFinalizer(farmSession).Finalize();
		delete farmSession;
	}
	 
	
}