#ifndef __COLLECTORSERVERINTERFACE_H__
#define __COLLECTORSERVERINTERFACE_H__

#include <arrays/Enumerator.h>
#include "ICollector.h"
#include "HerdSnapshot.h"
#include "HerdModifications.h"

class CollectorServerInterface
{
public:
	virtual PtrEnumerator<ICollector> EnumCollectors() = 0;
	virtual void Upload(uint32_t ccollectorId,HerdSnapshot* snapshot) = 0;
	virtual HerdModifications Download(uint32_t ccollectorId) = 0;

};

#endif
