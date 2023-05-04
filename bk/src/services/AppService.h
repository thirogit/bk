#ifndef __APPSERVICE_H__
#define __APPSERVICE_H__


#include "ServiceRegistry.h"

class AppService
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Init(ServiceRegistry* registry) = 0;
	virtual ~AppService() {};
};






#endif