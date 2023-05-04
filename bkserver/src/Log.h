#pragma once

#include <boost/format.hpp>

class Log
{
public:
	static void Configure();
	//static void D(const char* szMessage);
	//static void I(const std::string& sTag,LogMessage& msg);
	static void I(const std::wstring& sTag,const std::wstring& sMessage);

private:
	static Log m_instance;
};