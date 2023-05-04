#ifndef __COLLECTORTEXT_H__
#define __COLLECTORTEXT_H__

#include "../../ICollector.h"


struct CollectorMember
{
	enum
	{
		OsVersion,
		DeviceName,
		ClientVersion,
		IpAddress,
		Status,		
		CollectorMember_LastMember
	};
};

class CollectorText 
{
public:
	virtual std::wstring Text(const ICollector *pCollector, uint32_t memberId);
};

#endif