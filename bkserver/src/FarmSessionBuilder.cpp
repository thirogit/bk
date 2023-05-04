#include "stdafx.h"
#include "FarmSessionBuilder.h"
#include "runtime/BuildException.h"
#include "db/SQLException.h"

FarmSessionBuilder::FarmSessionBuilder(Farm* pFarm, gcp::GoogleServiceAccount& gaccount, const repo::RepoConnectionSettings& repoSettings) : 
	m_pFarm(pFarm), 
	m_gaccount(gaccount),
	m_repoSettings(repoSettings)
{
}

FarmSessionBuilder::~FarmSessionBuilder() 
{
}

FarmSession* FarmSessionBuilder::Build()
{
	NewPtrFlushArray<User> users;
	NewPtrFlushArray<Season> seasons;
	NewPtrFlushArray<Herd> herds;
	NewPtrFlushArray<Agent> agents;
	NewPtrFlushArray<Class> classes;
	NewPtrFlushArray<Stock> stocks;
	NewPtrFlushArray<BankAccount> bankaccounts;
	std::auto_ptr<FarmSession> session(new FarmSession());
	session->m_pFarm = m_pFarm;
	
	try
	{
		m_dao.LoadUsers(users, m_pFarm);
		m_dao.LoadSeasons(seasons, m_pFarm);
		m_dao.LoadHerds(herds, m_pFarm);
		m_dao.LoadAgents(agents, m_pFarm->GetId());
		m_dao.LoadBankAccounts(bankaccounts, m_pFarm->GetId());
		m_dao.LoadClasses(classes, m_pFarm->GetId());
		m_dao.LoadStocks(stocks, m_pFarm->GetId());
	}
	catch (SQLException& e)
	{
		BOOST_THROW_EXCEPTION(BuildException(e.message()));
	}

	Herd* herd = NULL;
	for (auto herdIt = herds.begin(); herdIt != herds.end(); herdIt++)
	{
		herd = *herdIt;
		session->m_Herds.insert(herd);
	}

	Season* pSeason;
	for (auto seasonIt = seasons.begin(); seasonIt != seasons.end(); seasonIt++)
	{
		pSeason = *seasonIt;
		session->m_Seasons[pSeason->GetId()] = pSeason;
	}

	Agent* agent = NULL;
	for (auto agentIt = agents.begin(); agentIt != agents.end(); agentIt++)
	{
		agent = *agentIt;
		session->m_Agents.insert(agent);
	}
	
	for (auto userIt = users.begin(); userIt != users.end(); userIt++)
	{
		session->m_Users.insert(*userIt);
	}

	for (auto accountIt = bankaccounts.begin(); accountIt != bankaccounts.end(); accountIt++)
	{
		session->m_Accounts.add(*accountIt);
	}

	std::copy(classes.begin(), classes.end(), std::inserter(session->m_Classes, session->m_Classes.begin()));
	std::copy(stocks.begin(), stocks.end(), std::inserter(session->m_Stocks, session->m_Stocks.begin()));

	session->m_Audience = new FarmAudience(m_pFarm->GetFarmNo(), new CloudPublisher(m_gaccount));
	session->m_log = new BkLogger(m_pFarm->GetFarmNo().ToString());
	session->m_RepoClientFactory = new repo::RepoClientFactory(m_repoSettings);
	session->m_ChangePublisherFactory = new ChangePublisherFactory(m_gaccount, session.get());

	return session.release();
}
