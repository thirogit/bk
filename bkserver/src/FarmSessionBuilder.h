#pragma once

#include "FarmSession.h"
#include "dao/MuuDAO.h"
#include "cloud/GoogleServiceAccount.h"
#include "repo/RepoClientFactory.h"
#include "cloud/GoogleServiceAccount.h"

class FarmSessionBuilder
{
public:
	FarmSessionBuilder(Farm* pFarm, gcp::GoogleServiceAccount& gaccount, const repo::RepoConnectionSettings& repoSettings);
	~FarmSessionBuilder();	
	FarmSession* Build();
private:
	MuuDAO m_dao;
	Farm* m_pFarm;
	gcp::GoogleServiceAccount m_gaccount;
	repo::RepoConnectionSettings m_repoSettings;
};
