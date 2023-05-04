#include "stdafx.h"
#include "FarmSessionFinalizer.h"
#include "FarmSession.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include "SeasonSessionFinalizer.h"

FarmSessionFinalizer::FarmSessionFinalizer(FarmSession* session) : m_session(session)
{
}

void FarmSessionFinalizer::Finalize()
{
	if (m_session->m_Audience)
	{
		m_session->m_Audience->Finish();
		delete m_session->m_Audience;
		m_session->m_Audience = NULL;
	}

	delete m_session->m_pFarm;
	delete m_session->m_log;
	delete m_session->m_ChangePublisherFactory;
	DeepDelete(m_session->m_Seasons);
	DeepDelete(m_session->m_Herds);
	DeepDelete(m_session->m_Users);
	DeepDelete(m_session->m_Agents);
	DeepDelete(m_session->m_Classes);
	DeepDelete(m_session->m_Stocks);

	BOOST_FOREACH(SeasonSession* session,m_session->m_SeasonSessions | boost::adaptors::map_values)
	{
		SeasonSessionFinalizer(session).Finalize();
		delete session;
	}

	BOOST_FOREACH(UserSession* session, m_session->m_UserSessions | boost::adaptors::map_values)
	{
		session->End();
		delete session;
	}


}
