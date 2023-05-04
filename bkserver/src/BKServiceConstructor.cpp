#include "stdafx.h"
#include "BKServiceConstructor.h"
#include "dao/MuuDAO.h"
#include "FarmSessionBuilder.h"
#include "db/SQLException.h"
#include <algorithm>
#include "Log.h"
#include <boost/foreach.hpp>
#include "runtime/BuildException.h"
#include "FarmSessionFinalizer.h"
#include "cloud/CloudPublisher.h"

BKServiceConstructor::BKServiceConstructor(BKService* pService,const gcp::GoogleServiceAccount& gaccount, const repo::RepoConnectionSettings& repoSettings) :
	m_pService(pService), m_gaccount(gaccount), m_repoSettings(repoSettings)
{
}

void BKServiceConstructor::Construct(const std::vector<FarmNo>& farmNos)
{
	MuuDAO dao;

	NewPtrFlushArray<FarmSession> farmSessions;
	NewPtrFlushArray<Farm> farms;


	BOOST_FOREACH(FarmNo farmNo, farmNos)
	{
		Farm* farm = dao.LoadFarm(farmNo);

		if (!farm)
		{
			DeepDelete(farms);
			BOOST_THROW_EXCEPTION(BuildException(L"unable to load farm " + farmNo.ToString()));
		}

		farms.push_back(farm);
	}
	
	Farm* pFarm;
	NewPtrFlushArray<Farm>::iterator farmIt = farms.begin();
	
	try
	{
		while (farmIt != farms.end())
		{
			pFarm = *farmIt;

			FarmSessionBuilder builder(pFarm, m_gaccount, m_repoSettings);

			farmSessions.Add(builder.Build());

			farmIt++;
		}
	}
	catch (BuildException& e)
	{
		BOOST_FOREACH(FarmSession* session, farmSessions)
		{
			FarmSessionFinalizer(session).Finalize();
			delete session;
		}
		
		std::rethrow_exception(std::current_exception());
	}

	std::for_each(farmSessions.begin(), farmSessions.end(),
		[this](FarmSession* pFarmSession)
	{
		m_pService->m_FarmSessions[pFarmSession->GetFarm()->GetFarmNo()] = pFarmSession;
	});

	
	
}
