#pragma once

#include "SeasonObserver.h"
#include <boost/shared_ptr.hpp>

class Streak : public SeasonObserver
{

};

class ObserverTransaction : public SeasonObserver
{
public:
	virtual void Commit() = 0;
	virtual void Rollback() = 0;
	virtual Streak* StartStreak() = 0; 
	virtual ~ObserverTransaction() = 0;
};

typedef boost::shared_ptr<ObserverTransaction> ObserverTransactionPtr;

class SeasonSpectator
{
public:
	virtual uint16_t GetPort() = 0;
	virtual void Release() = 0;
	virtual ObserverTransactionPtr OpenTransaction() = 0;
	
};