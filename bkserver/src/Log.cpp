#include "stdafx.h"
#include "Log.h"

//#include <log4cxx/logger.h>
//#include <log4cxx/consoleappender.h>
//#include <log4cxx/simplelayout.h>
//#include <log4cxx/logmanager.h>
//#include <string\TextUtils.h>
//
//
//using namespace log4cxx;
//using namespace log4cxx::helpers;

Log Log::m_instance;

void Log::Configure()
{
	/*if(!LogManager::getLoggerRepository()->isConfigured())
	{
		log4cxx::ConsoleAppenderPtr appender(new log4cxx::ConsoleAppender());
		log4cxx::LayoutPtr layout(new log4cxx::SimpleLayout());
		appender->setLayout(layout);
		log4cxx::helpers::Pool pool;
		appender->activateOptions(pool);
		log4cxx::Logger::getRootLogger()->addAppender(appender);
		LogManager::getLoggerRepository()->setConfigured(true);
	}*/
}

//void Log::D(const char* szMessage)
//{
	//log4cxx::Logger::getRootLogger()->debug(szMessage);
	
//}

//void Log::I(const std::string& sTag,LogMessage& msg)
//{
	//log4cxx::Logger::getRootLogger()->info(sTag + ": " + msg.str());
//}

void Log::I(const std::wstring& sTag,const std::wstring& sMessage)
{
	//log4cxx::Logger::getRootLogger()->info(szMessage);
//	std::wcout << sTag << L": " << sMessage << std::endl;
}
