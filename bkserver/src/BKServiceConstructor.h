#pragma once

#include "BKService.h"

class BKServiceConstructor
{
public:
	BKServiceConstructor(BKService* pService,const gcp::GoogleServiceAccount& gaccount, const repo::RepoConnectionSettings& repoSettings);
	void Construct(const std::vector<FarmNo>& farmNos);
private:
	BKService* m_pService;
	gcp::GoogleServiceAccount m_gaccount;
	repo::RepoConnectionSettings m_repoSettings;
};