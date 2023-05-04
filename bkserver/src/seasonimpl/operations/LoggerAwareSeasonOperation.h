#ifndef __LOGGERAWARESEASONOPERATION_H__
#define __LOGGERAWARESEASONOPERATION_H__

#include "SeasonOperation.h"
#include "../../log/LoggerAware.h"

class LoggerAwareSeasonOperation : public SeasonOperation, protected LoggerAware
{
public:
	LoggerAwareSeasonOperation(SeasonSession* session, LogInterface* logger);
};

#endif
