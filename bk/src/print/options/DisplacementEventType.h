#ifndef __DISPLACEMENTEVENTTYPE_H__
#define __DISPLACEMENTEVENTTYPE_H__

#include <stdint.h>

enum DisplacementEventType : uint32_t
{
	DisplacementEventType_None = 0,
	DisplacementEventType_Buy = 1,
	DisplacementEventType_Sell = 2,
	DisplacementEventType_BuySell = 4,
	DisplacementEventType_ExportToEU = 8,
	DisplacementEventType_ExportNotToEU = 16,
	DisplacementEventType_Other = 32,
	
};

#endif
