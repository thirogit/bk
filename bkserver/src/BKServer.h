#ifndef __BKSERVER_H__
#define __BKSERVER_H__

#include <RCF/RCF.hpp>
#include "BKService.h"

class BKServer
{
public:
	void Initialize(uint16_t port, const std::vector<FarmNo>& farmNums,const gcp::GoogleServiceAccount& gaccount, const repo::RepoConnectionSettings& repoSettings);
	void Finalize();
private:
	RCF::RcfInitDeinit rcfInit;
	RCF::RcfServer rcfServer;
	BKService bkService;
	
	
};


#endif