#ifndef __KEYSINPUTINTERFACE_H__
#define __KEYSINPUTINTERFACE_H__

#include "../context/SeasonSession.h"
#include "../runtime/UniquePresenceKeys.h"

class KeysInputInterface
{
public:
	virtual void Feed(SeasonSession* pSession,const UniquePresenceKeys& keys) = 0;
};


#endif
