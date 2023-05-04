#ifndef __INSERTHENTOPERATION_H__
#define __INSERTHENTOPERATION_H__

#include "LoggerAwareSeasonOperation.h"

class InsertHentOperation : public LoggerAwareSeasonOperation
{
public:
	InsertHentOperation(IHent* hent,SeasonSession* session,LogInterface* logger);
	virtual void execute();
protected:
	IHent* m_hent;
};

#endif
