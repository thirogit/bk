#pragma once

#include <stdint.h>
#include <map>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include "ClientSeasonSessionFacade.h"
#include <configuration/print/PrintProfiles.h>
#include <configuration/passportrecognition/PassportRecognitionProfiles.h>
#include <configuration/deductions/DeductionDefinitions.h>
#include "UserSessionFacade.h"
#include "FarmSession.h"
#include "BkLogger.h"

class SeasonSession;


class ClientSession
{
	struct index_tag_seasonid{};
	struct index_tag_token{};
		
	typedef boost::multi_index_container
	<
	  ClientSeasonSessionFacade,
	  boost::multi_index::indexed_by< 
	  boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_token>, boost::multi_index::const_mem_fun<ClientSeasonSessionFacade,uint32_t, &ClientSeasonSessionFacade::GetSessionToken> >,
      boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_seasonid>, boost::multi_index::const_mem_fun<ClientSeasonSessionFacade,uint32_t, &ClientSeasonSessionFacade::GetSeasonId> >
									 >
	> ClientSeasonSessionsIndex;

	typedef ClientSeasonSessionsIndex::nth_index<0>::type ClientSeasonSessionsByToken;

	//typedef boost::range_detail::any_iterator<ClientSeasonSessionFacade, boost::forward_traversal_tag, ClientSeasonSessionFacade, std::ptrdiff_t > ClientSeasonSessionIterator;


public:
	ClientSession(FarmSession* pOwnerSession,uint32_t userId);
	~ClientSession();
	ClientSeasonSessionFacade OpenSeason(uint32_t seasonId);
	ClientSeasonSessionFacade FindClientSeasonSession(uint32_t sessionToken);
	uint32_t GetUserId() const;
	void CloseSession(uint32_t sessionToken);
	void CloseAllSessions();
	repo::RepoClientFactory* GetRepoClientFactory();
	
	template<typename F>
	void UserDoAny(const F& f)
	{
		std::function<void(UserSessionFacade*)> job = f;
		m_pOwnerSession->SubmitUserJob(m_userId,job);
	}

	template<typename R, typename F>
	R UserEvalAny(F functor)
	{		
		std::function< R(UserSessionFacade*)>  f = functor;
		return UserEval<R>(f);
	}

	template<typename R>
	R UserEval(std::function< R(UserSessionFacade*)> & f)
	{
		typedef std::packaged_task<R(UserSessionFacade*)> eval_t;
		eval_t evaluator(f);
		std::function<void(UserSessionFacade*)> evalFunctor = std::bind(&eval_t::operator(), &evaluator, std::placeholders::_1);
		m_pOwnerSession->SubmitUserJob(m_userId,evalFunctor);
		return evaluator.get_future().get();
	}
	
	PrintProfiles FetchPrintProfiles();
	void UpdatePrintProfile(IPrintProfile* pProfile);
	uint32_t NewPrintProfile(IPrintProfile* pProfile);
	void DeletePrintProfile(uint32_t profileId);

	PassportRecognitionProfiles FetchPassportRecognitionProfiles();
	void UpdatePassportRecognitionProfile(IPassportRecognitionProfile* pProfile);
	uint32_t NewPassportRecognitionProfile(IPassportRecognitionProfile* pProfile);
	void DeletePassportRecognitionProfile(uint32_t profileId);

	DeductionDefinitions FetchDeductionDefinitions();
	void UpdateDeductionDefinition(IDeductionDefinition* pDefinition);
	uint32_t NewDeductionDefinition(IDeductionDefinition* pDefinition);
	void DeleteDeductionDefinition(uint32_t definitionId);

	void d(const std::wstring& message);
	void i(const std::wstring& message);
	void e(const std::wstring& message);

	UserSettings GetSettings();
	void SaveSettings(const UserSettings& settings);

private:
	
private:
	BkLogger* m_logger;
	FarmSession* m_pOwnerSession;
	uint32_t m_userId;
	ClientSeasonSessionsIndex m_ClientSeasonSessions;
};
