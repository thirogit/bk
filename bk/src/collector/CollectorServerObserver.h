#ifndef __COLLECTORSERVEROBSERVER_H__
#define __COLLECTORSERVEROBSERVER_H__

#include <cstdint>

class CollectorServerObserver
{
public:
	virtual void CollectorConnected(uint32_t id) = 0;
	virtual void CollectorUpdated(uint32_t id) = 0;
	virtual void CollectorDisconnected(uint32_t id) = 0;
};

#endif
