#include "stdafx.h"
#include <log4cxx/logstring.h>
#include "XLogger.h"
#include <log4cxx/level.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/spi/location/locationinfo.h>
#include <log4cxx/spi/loggerrepository.h>


using namespace log4cxx;
using namespace log4cxx::spi;

IMPLEMENT_LOG4CXX_OBJECT(XLogger)
IMPLEMENT_LOG4CXX_OBJECT(XFactory)

XFactoryPtr XLogger::factory = new XFactory();



LoggerPtr XLogger::getLogger(const LogString& name)
{
        return LogManager::getLogger(name, factory);
}

LoggerPtr XLogger::getLogger(const helpers::Class& clazz)
{
        return XLogger::getLogger(clazz.getName());
}



XFactory::XFactory()
{
}

LoggerPtr XFactory::makeNewLoggerInstance(log4cxx::helpers::Pool& pool, 
       const LogString& name) const
{
        return new XLogger(pool, name);
}
