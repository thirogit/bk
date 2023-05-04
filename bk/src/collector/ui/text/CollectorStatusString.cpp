#include "stdafx.h"
#include "CollectorStatusString.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"

	
CollectorStatusString::CollectorStatusString(CollectorDeviceStatus status)
{
	switch(status)
	{
		case CollectorDeviceStatus_Busy:
			std::wstring::operator=(StringLoader(IDS_COLLECTORSTATUS_BUSY));
			break;
		case CollectorDeviceStatus_Syncing:
			std::wstring::operator=(StringLoader(IDS_COLLECTORSTATUS_SYNCING));
			break;
		case CollectorDeviceStatus_ReadyToSync:
			std::wstring::operator=(StringLoader(IDS_COLLECTORSTATUS_READY2SYNC));
			break;
		case CollectorDeviceStatus_Idle:
			std::wstring::operator=(StringLoader(IDS_COLLECTORSTATUS_IDLE));
			break;
		
	}	
}
	
const std::wstring& CollectorStatusString::ToString()
{
	return *this;  	
}


