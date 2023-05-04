#pragma once

#include <string>
#include "log/LogInterface.h"
#include <log4cxx/logger.h>

class BkLogger
{
public:
	BkLogger(const std::wstring& name);
	~BkLogger();

	void d(const std::wstring& message);
	void i(const std::wstring& message);
	void e(const std::wstring& message);
	
private:
	std::wstring m_name;
	log4cxx::LoggerPtr m_logger;
	log4cxx::helpers::Pool m_pool;
};

class BkLoggerAdapter : public LogInterface
{
public:
	BkLoggerAdapter(BkLogger* logger);
	virtual void Log(LogLevel level, const std::wstring& msg);
private:
	BkLogger* m_logger;
	
};
