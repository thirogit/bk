#ifndef __LOGGERAWARE_H__
#define __LOGGERAWARE_H__

#include "LogInterface.h"
#include <string/TextUtils.h>

class LoggerAware
{
public:
	LoggerAware(LogInterface* log);
	
	void Log(LogLevel level, const std::wstring& msg)
	{
		m_log->Log(level, msg);
	}

	template<typename A1>
	void Log(LogLevel level, const std::wstring& msg, A1 a)
	{
		m_log->Log(level, TextUtils::Format(msg, a));
	}

	template<typename A1,typename A2>
	void Log(LogLevel level, const std::wstring& msg, A1 a1, A2 a2)
	{
		boost::fusion::vector2<A1,A2> parameters(a1,a2); 
		m_log->Log(level, TextUtils::FormatFusion(msg, parameters));
	}

	template<typename A1,typename A2,typename A3>
	void Log(LogLevel level, const std::wstring& msg, A1 a1, A2 a2, A3 a3)
	{
		boost::fusion::vector3<A1,A2,A3> parameters(a1,a2,a3); 
		m_log->Log(level, TextUtils::FormatFusion(msg, parameters));
	}

	template<typename A1, typename A2, typename A3, typename A4>
	void Log(LogLevel level, const std::wstring& msg, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		boost::fusion::vector4<A1, A2, A3, A4> parameters(a1, a2, a3, a4);
		m_log->Log(level, TextUtils::FormatFusion(msg, parameters));
	}

	template<typename A1, typename A2, typename A3, typename A4, typename A5>
	void Log(LogLevel level, const std::wstring& msg, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		boost::fusion::vector5<A1, A2, A3, A4, A5> parameters(a1, a2, a3, a4, a5);
		m_log->Log(level, TextUtils::FormatFusion(msg, parameters));
	}
	
	template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void Log(LogLevel level, const std::wstring& msg, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		boost::fusion::vector6<A1, A2, A3, A4, A5, A6> parameters(a1, a2, a3, a4, a5, a6);
		m_log->Log(level, TextUtils::FormatFusion(msg, parameters));
	}


	
	
private:
	LogInterface* m_log;
};

#endif