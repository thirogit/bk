#ifndef __COLLECTORDEVICESTATUS_H__
#define __COLLECTORDEVICESTATUS_H__


enum CollectorDeviceStatus : char
{
	CollectorDeviceStatus_Busy = 'B',
	CollectorDeviceStatus_Idle = 'I',
	CollectorDeviceStatus_Syncing = 'S',
	CollectorDeviceStatus_ReadyToSync = 'R',
	CollectorDeviceStatus_None = '\0'
};


#endif
