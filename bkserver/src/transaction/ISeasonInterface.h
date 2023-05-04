#pragma once

#include "../SeasonSessionFacade.h"
#include "../UserSessionFacade.h"
#include <future>

class ISeasonInterface
{
public:
	
	virtual void Do(std::function<void(SeasonSessionFacade*)>& action) = 0;
	virtual void DoUser(std::function<void(UserSessionFacade*)>& action) = 0;

	template<typename F>
	void DoAny(const F& f)
	{
		std::function<void(SeasonSessionFacade*)> job = f;
		Do(job);
	}

	template<typename R>
	R Eval(std::function< R (SeasonSessionFacade*)> & f)
	{
		typedef std::packaged_task<R (SeasonSessionFacade*)> eval_t;
		eval_t evaluator(f);	
		std::function<void (SeasonSessionFacade*)> evalFunctor = std::bind(&eval_t::operator(),&evaluator,std::placeholders::_1);
		Do(evalFunctor);
		return evaluator.get_future().get();
	}

	template<typename R, typename F>
	R EvalAny(const F& functor)
	{
		std::function< R(SeasonSessionFacade*)>  f = functor;
		return Eval<R>(f);
	}

	template<typename R>
	R EvalUser(std::function< R(UserSessionFacade*)> & f)
	{
		typedef std::packaged_task<R(UserSessionFacade*)> eval_t;
		eval_t evaluator(f);
		std::function<void(UserSessionFacade*)> evalFunctor = std::bind(&eval_t::operator(), &evaluator, std::placeholders::_1);
		DoUser(evalFunctor);
		return evaluator.get_future().get();
	}
};
