#ifndef __COLLECTORSTATUSSTRING_H__
#define __COLLECTORSTATUSSTRING_H__

#include "../../CollectorDeviceStatus.h"
#include <string>


class CollectorStatusString : private std::wstring
{
public:	
	CollectorStatusString(CollectorDeviceStatus status);
	const std::wstring& ToString();
};

#endif