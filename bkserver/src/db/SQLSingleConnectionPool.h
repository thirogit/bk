#include "SingleConnectionPool.h"
//#include "fbserver\FBSvrPool.h"
#include "pgserver/PGSvrPool.h"


typedef SingleConnectionPool<PGSvrPool> SQLSingleConnectionPool;
