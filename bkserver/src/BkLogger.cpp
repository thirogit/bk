#include "stdafx.h"
#include "BkLogger.h"
#include <log4cxx/patternlayout.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/logstring.h>
#include <log4cxx/logger.h>

#include <cstdlib>
#include <filesystem/paths.h>
#include <string/TextUtils.h>
#include <types/DateTime.h>


BkLogger::BkLogger(const std::wstring& name) : m_name(name)
{

	m_logger = log4cxx::Logger::getLogger(m_name);

	log4cxx::LayoutPtr layout(new log4cxx::PatternLayout(L"\%-5p \%d: \%m\%n"));

	log4cxx::ConsoleAppenderPtr consoleAppender(new log4cxx::ConsoleAppender(layout, log4cxx::ConsoleAppender::getSystemOut()));
		
	log4cxx::RollingFileAppenderPtr appender(new log4cxx::RollingFileAppender());
	appender->setLayout(layout);
	
	std::wstring bklogPath;

	char* BKLOG = std::getenv("BKLOG");
	if (BKLOG != NULL)
	{		
		bklogPath = TextUtils::FromUTF8(BKLOG);		
	}
	else
	{
		bklogPath = paths_ConcatPath(paths_GetApplicationDir(), L"BKLOG");
	}

	paths_EnsurePathExist(bklogPath);
	DateTime now = DateTime::now();

	std::wstring logFileName = TextUtils::Format(L"%s.log", name);

	appender->setFile(paths_ConcatPath(bklogPath, logFileName));
	
	appender->activateOptions(m_pool);
	consoleAppender->activateOptions(m_pool);

	m_logger->addAppender(appender);	
	m_logger->addAppender(consoleAppender);
}

BkLogger::~BkLogger()
{
	m_logger->closeNestedAppenders();
	m_logger->removeAllAppenders();
	m_logger->getLoggerRepository()->resetConfiguration();
}

void BkLogger::d(const std::wstring& message)
{
	m_logger->debug(message);
}

void BkLogger::i(const std::wstring& message)
{
	m_logger->info(message);
}

void BkLogger::e(const std::wstring& message)
{
	m_logger->error(message);
}
//#################################################################################################

BkLoggerAdapter::BkLoggerAdapter(BkLogger* logger) : m_logger(logger)
{
}

void BkLoggerAdapter::Log(LogLevel level, const std::wstring& msg)
{
	switch(level)
	{
		case LogLevel_Debug:
			m_logger->d(msg);
			break;
		case LogLevel_Info:
			m_logger->i(msg);
			break;			
		case LogLevel_Error:
			m_logger->e(msg);
			break;
	}
}