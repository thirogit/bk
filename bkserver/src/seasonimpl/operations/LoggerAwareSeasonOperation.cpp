#include "stdafx.h"
#include "LoggerAwareSeasonOperation.h"


LoggerAwareSeasonOperation::LoggerAwareSeasonOperation(SeasonSession* session, LogInterface* logger) : SeasonOperation(session), LoggerAware(logger)
{
}

