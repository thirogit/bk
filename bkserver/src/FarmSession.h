#pragma once

#include <data/impl/Farm.h>
#include <data/impl/Herd.h>
#include <data/impl/Season.h>
#include <data/impl/User.h>
#include <arrays/NewPtrFlushArray.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/thread.hpp>
#include <arrays/Enumerator.h>
#include "SeasonSession.h"
#include "FarmAudience.h"
#include "UserSession.h"
#include "SessionTokenGenerator.h"
#include "index/AgentsIndex.h"
#include "index/HerdsIndex.h"
#include "index/UsersIndex.h"
#include "BkLogger.h"
#include "repo/RepoClientFactory.h"
#include "cloud/ChangePublisher.h"

class FarmSessionBuilder;

class FarmSession
{
	class SessionTokenGeneratorImpl : public SessionTokenGenerator
	{
	public:
		virtual uint32_t ObtainNextToken();
		boost::atomic<uint32_t> sequence;
	};
	
	typedef std::map<uint32_t,Season*> SeasonsIndex;

public:
	FarmSession();
	User* Authenticate(const std::wstring& sUser,const std::wstring& sPassword);
	SeasonSession* AquireSeasonSession(uint32_t seasonId);	
	IFarm* GetFarm() const;
	PtrEnumerator<IHerd> EnumHerds() const;
	PtrEnumerator<ISeason> EnumSeasons() ;
	PtrEnumerator<IAgent> EnumAgents();
	PtrEnumerator<IClass> EnumClasses();
	PtrEnumerator<IStock> EnumStocks();
	PtrEnumerator<IBankAccount> EnumAccounts();
	void EndFetch(uint32_t sessionToken);
	FarmAudience* GetAudience();
	IHerd* FindHerdById(uint32_t) const;
	IHerd* FindHerdByIndex(int) const;
	IAgent* FindAgent(uint32_t agentId);
	IAgent* FindAgent(const std::wstring& agentCd);
	IUser* FindUser(uint32_t userId);
	IClass* FindClass(uint32_t classId);
	IStock* FindStock(uint32_t stockId);
	IClass* FindClass(const std::wstring& classCd);
	IStock* FindStock(const std::wstring& stockCd);	
	
	void SubmitUserJob(uint32_t userId,std::function<void(UserSessionFacade*)>& job);
	void BeginUserSession(uint32_t userId);
	repo::RepoClientFactory* GetRepoClientFactory();
	ChangePublisherFactory* GetChangePublisherFactory();

private:
	BkLogger* m_log;
	Farm* m_pFarm;
	SeasonsIndex m_Seasons;
	HerdsIndex m_Herds;
	UsersIndex m_Users;
	AgentsIndex	m_Agents;
	ClassesIndex m_Classes;
	StocksIndex	m_Stocks;
	
	BankAccountIndex m_Accounts;
	

	std::map<uint32_t, UserSession*> m_UserSessions;
	boost::shared_mutex m_UserSessionsMutex;

	std::map<uint32_t,SeasonSession*> m_SeasonSessions;
	boost::shared_mutex m_SeasonSessionsMutex;
		
	FarmAudience* m_Audience;
	repo::RepoClientFactory* m_RepoClientFactory;
	ChangePublisherFactory* m_ChangePublisherFactory;

	SessionTokenGeneratorImpl m_tokenGenerator;

	friend class FarmSessionBuilder;
	friend class FarmSessionFinalizer;
};
