#include "stdafx.h"
#include "BKServer.h"
#include "BKServiceConstructor.h"
#include "BKServiceDestructor.h"
#include <service/IBKService.h>


void BKServer::Initialize(uint16_t port,const std::vector<FarmNo>& farmNums, const gcp::GoogleServiceAccount& account,const repo::RepoConnectionSettings& repoSettings)
{
	BKServiceConstructor constructor(&bkService, account, repoSettings);
	constructor.Construct(farmNums);

	rcfServer.addEndpoint(RCF::TcpEndpoint("0.0.0.0", port));
	rcfServer.bind<IBKService>(bkService);
	
	RCF::ThreadPoolPtr tpPtr( new RCF::ThreadPool(5) );
	rcfServer.setThreadPool(tpPtr);
	
	rcfServer.start();
	
}

void BKServer::Finalize()
{
	rcfServer.stop();
	BKServiceDestructor destructor(&bkService);
	destructor.Destroy();	
}

