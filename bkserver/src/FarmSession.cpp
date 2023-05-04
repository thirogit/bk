#include "stdafx.h"
#include "FarmSession.h"
#include "SeasonSessionBuilder.h"
#include <boost/range/adaptor/map.hpp>
#include "UserSessionBuilder.h"
#include <string/TextUtils.h>

uint32_t FarmSession::SessionTokenGeneratorImpl::ObtainNextToken()
{
	return sequence.fetch_add(1);
};

//#################################################################################################

FarmSession::FarmSession() :  m_pFarm(NULL), m_Audience(NULL)
{
	m_tokenGenerator.sequence = 1000;
}

IFarm* FarmSession::GetFarm() const
{
	return m_pFarm;
}


PtrEnumerator<IHerd> FarmSession::EnumHerds() const
{
	return PtrEnumerator<IHerd>(m_Herds);
}

IHerd* FarmSession::FindHerdById(uint32_t herdId) const
{
	const boost::multi_index::index<HerdsIndex, herdindex_tag_herdId>::type& indexById = boost::multi_index::get<herdindex_tag_herdId>(m_Herds);

	auto herdIt = indexById.find(herdId);
	if (herdIt != indexById.end())
		return *herdIt;

	return NULL;
}

IHerd* FarmSession::FindHerdByIndex(int herdIndex) const
{	
	const boost::multi_index::index<HerdsIndex, herdindex_tag_herdIndex>::type& indexByIndex = boost::multi_index::get<herdindex_tag_herdIndex>(m_Herds);

	auto herdIt = indexByIndex.find(herdIndex);
	if (herdIt != indexByIndex.end())
		return *herdIt;

	return NULL;
}

PtrEnumerator<ISeason> FarmSession::EnumSeasons() 
{	
	return PtrEnumerator<ISeason>((m_Seasons | boost::adaptors::map_values));		
}

User* FarmSession::Authenticate(const std::wstring& sUser,const std::wstring& sPassword)
{	
	const boost::multi_index::index<UsersIndex,userindex_tag_username>::type& usersByName = 
		boost::multi_index::get<userindex_tag_username>(m_Users);
	
	auto userIt = usersByName.find(sUser);

	if(userIt != usersByName.end())
	{
		User* pUser = (*userIt);
		if (pUser->GetPassword().compare(sPassword) == 0)
		{
			BeginUserSession(pUser->GetId());
			return pUser;
		}
	}		
	return NULL;		
}

SeasonSession* FarmSession::AquireSeasonSession(uint32_t seasonId)
{
	boost::shared_lock<boost::shared_mutex> rlock(m_SeasonSessionsMutex);
	auto seasonIt = m_Seasons.find(seasonId);
	if(seasonIt != m_Seasons.end())
	{
		auto sessionIt = m_SeasonSessions.find(seasonId);
		if(sessionIt == m_SeasonSessions.end())
		{
			rlock.unlock();
			boost::upgrade_lock<boost::shared_mutex> lock(m_SeasonSessionsMutex);
			boost::upgrade_to_unique_lock<boost::shared_mutex> wLock(lock);
			
			sessionIt = m_SeasonSessions.find(seasonId);
			if(sessionIt == m_SeasonSessions.end())
			{
				m_log->d(L"loading season: " + seasonIt->second->GetSeasonName());
				SeasonSessionBuilder builder(m_log);
				builder.SetSeason(seasonIt->second);
				builder.SetParentSession(this);
				builder.SetTokenGenerator(&m_tokenGenerator);
				SeasonSession* pNewSession = builder.Build();
				m_SeasonSessions[seasonId] = pNewSession;

				m_log->d(L"season " + seasonIt->second->GetSeasonName() + L" loaded, observer port: " + TextUtils::Format(L"%d",pNewSession->GetObserverPort()));

				return pNewSession;
			}
			else
			{
				return sessionIt->second;
			}
		}
		else
		{
			return sessionIt->second;
		}
	}
	return NULL;
}

FarmAudience* FarmSession::GetAudience()
{
	return m_Audience;
}

void FarmSession::BeginUserSession(uint32_t userId)
{
	boost::shared_lock<boost::shared_mutex> rlock(m_UserSessionsMutex);
	auto userSessionIt = m_UserSessions.find(userId);
	if (userSessionIt == m_UserSessions.end())
	{
		rlock.unlock();
		boost::upgrade_lock<boost::shared_mutex> lock(m_UserSessionsMutex);
		boost::upgrade_to_unique_lock<boost::shared_mutex> wLock(lock);

		m_log->d(TextUtils::Format(L"starting user session: userid=%d", userId));
		userSessionIt = m_UserSessions.find(userId);
		if (userSessionIt == m_UserSessions.end())
		{
			UserSessionBuilder builder(userId);
			UserSession* session = builder.Build();			
			m_UserSessions[userId] = session;
			session->Begin();
		}		
	}	
}

void FarmSession::SubmitUserJob(uint32_t userId, std::function<void(UserSessionFacade*)>& job)
{
	boost::shared_lock<boost::shared_mutex> rlock(m_UserSessionsMutex);
	auto userSessionIt = m_UserSessions.find(userId);
	if (userSessionIt != m_UserSessions.end())
	{
		userSessionIt->second->Do(job);
	}
}

IAgent* FarmSession::FindAgent(const std::wstring&  agentCd)
{
	const boost::multi_index::index<AgentsIndex, agentsindex_tag_code>::type& indexByCode = boost::multi_index::get<agentsindex_tag_code>(m_Agents);

	auto it = indexByCode.find(agentCd);

	if (it != indexByCode.end())
		return *it;

	return NULL;
}

IAgent*  FarmSession::FindAgent(uint32_t agentId)
{
	auto agentIt = m_Agents.find(agentId);
		if (agentIt != m_Agents.end())
			return *agentIt;
	return NULL;
}

PtrEnumerator<IAgent> FarmSession::EnumAgents()
{
	return PtrEnumerator<IAgent>(m_Agents);
}

IUser* FarmSession::FindUser(uint32_t userId)
{
	auto userIt = m_Users.find(userId);
	if (userIt != m_Users.end())
		return *userIt;

	return NULL;
}

PtrEnumerator<IBankAccount> FarmSession::EnumAccounts()
{
	return PtrEnumerator<IBankAccount>(m_Accounts);
}

IClass* FarmSession::FindClass(uint32_t classId)
{
	auto classIt = m_Classes.find(classId);
	if (classIt != m_Classes.end())
		return *classIt;

	return NULL;
}

IStock* FarmSession::FindStock(uint32_t stockId)
{
	auto stockIt = m_Stocks.find(stockId);
	if (stockIt != m_Stocks.end())
		return *stockIt;

	return NULL;
}

IClass* FarmSession::FindClass(const std::wstring& classCd)
{
	const boost::multi_index::index<ClassesIndex, classesindex_tag_code>::type& indexByCode = boost::multi_index::get<classesindex_tag_code>(m_Classes);

	auto it = indexByCode.find(classCd);

	if (it != indexByCode.end())
		return *it;

	return NULL;
}

IStock* FarmSession::FindStock(const std::wstring& stockCd)
{
	const boost::multi_index::index<StocksIndex, stocksindex_tag_code>::type& indexByCode = boost::multi_index::get<stocksindex_tag_code>(m_Stocks);

	auto it = indexByCode.find(stockCd);

	if (it != indexByCode.end())
		return *it;

	return NULL;
}


PtrEnumerator<IClass> FarmSession::EnumClasses()
{
	return PtrEnumerator<IClass>(m_Classes);
}

PtrEnumerator<IStock> FarmSession::EnumStocks()
{
	return PtrEnumerator<IStock>(m_Stocks);
}

repo::RepoClientFactory* FarmSession::GetRepoClientFactory()
{
	return m_RepoClientFactory;
}

ChangePublisherFactory* FarmSession::GetChangePublisherFactory()
{
	return m_ChangePublisherFactory;
}