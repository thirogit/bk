#pragma once

#include <data/impl/Season.h>
#include <data/impl/Class.h>
#include <data/impl/Stock.h>
#include <data/impl/Farm.h>
#include "SeasonSessionFacade.h"
#include <future>
#include "IFetchHandle.h"

class ClientSeasonSession;

class ClientSeasonSessionInterface
{
public:
	virtual void Close(ClientSeasonSession* pSession) = 0;
	virtual ISeason* GetSeason() const = 0;
	virtual IFarm* GetFarm() const = 0;
	virtual IClass* FindClass(uint32_t classId) = 0;
	virtual IStock* FindStock(uint32_t stockId) = 0;
	virtual IHerd* FindHerd(uint32_t herdId) = 0;		
	virtual IAgent* FindAgent(uint32_t agentId) = 0;		
	virtual uint16_t GetObserverPort() const = 0;
	virtual FetchHandle OpenFetchHandle() = 0;		
	virtual void Do(ClientSeasonSession* pClient,std::function<void (SeasonSessionFacade*)>& action) = 0;

	template<typename F> void DoAny(ClientSeasonSession* pClient,F const& f)
	{
		std::function<void (SeasonSessionFacade*)> action = f;
		Do(pClient,action);
	}

	template<typename R>
	R Eval(ClientSeasonSession* pClient,std::function< R (SeasonSessionFacade*)> & f)
	{
		typedef std::packaged_task<R (SeasonSessionFacade*)> eval_t;
		eval_t evaluator(f);	
		std::function<void (SeasonSessionFacade*)> evalFunctor = std::bind(&eval_t::operator(),&evaluator,std::placeholders::_1);
		Do(pClient,evalFunctor);
		return evaluator.get_future().get();
	}

	template<typename R,typename F>
	R EvalAny(ClientSeasonSession* pClient,F functor)
	{
		std::function<R (SeasonSessionFacade*)> evalFunctor = functor;
		return Eval(pClient,evalFunctor);
	}

};
