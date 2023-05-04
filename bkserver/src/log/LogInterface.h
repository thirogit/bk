#ifndef __LOGINTERFACE_H__
#define __LOGINTERFACE_H__

#include <string>


enum LogLevel
{
	LogLevel_Debug,
	LogLevel_Info,
	LogLevel_Error
};


class LogInterface
{
public:
	virtual void Log(LogLevel level,const std::wstring& msg) = 0;
};


#endif