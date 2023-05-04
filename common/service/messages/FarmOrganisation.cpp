#include "stdafx.h"
#include "FarmOrganisation.h"
#include <SF/vector.hpp> 
#include "../../data/impl/serialization/SerializableFarm.h"
#include "../../data/impl/serialization/SerializableHerd.h"
#include "../../data/impl/serialization/SerializableSeason.h"
#include "../../data/impl/serialization/SerializableAgent.h"
#include "../../data/impl/serialization/SerializableClass.h"
#include "../../data/impl/serialization/SerializableStock.h"
#include "../../data/impl/serialization/BankAccountSerializer.h"
#include "../../CommonTypes.h"

FarmOrganisation::FarmOrganisation() : m_pFarm(NULL)
{
}

FarmOrganisation::~FarmOrganisation() 
{
	Destroy();
}

void FarmOrganisation::Destroy()
{
	m_Seasons.FreeAndFlush();
	m_Herds.FreeAndFlush();
	m_Agents.FreeAndFlush();
	m_Accounts.FreeAndFlush();
	m_Classes.FreeAndFlush();
	m_Stocks.FreeAndFlush();
	delete m_pFarm;
	m_pFarm = NULL;
}

void FarmOrganisation::CopyFrom(const FarmOrganisation* src)
{
	Destroy();

	if (src->m_pFarm)
	{
		CreateFarm(src->m_pFarm->GetId())->CopyFrom(src->m_pFarm);
	}


	auto herdIt = src->m_Herds.begin();
	for (herdIt; herdIt != src->m_Herds.end(); herdIt++)
	{
		Herd* herd = *herdIt;
		AddHerd(herd->GetId())->CopyFrom(herd);
	}
	
	auto seasonIt = src->m_Seasons.begin();
	for (seasonIt; seasonIt != src->m_Seasons.end(); seasonIt++)
	{
		Season* season = *seasonIt;
		AddSeason(season->GetId())->CopyFrom(season);
	}

	
	auto agentIt = src->m_Agents.begin();
	for (agentIt; agentIt != src->m_Agents.end(); agentIt++)
	{
		Agent* agent = *agentIt;
		AddAgent(agent->GetId())->CopyFrom(agent);
	}

	
	auto accountIt = src->m_Accounts.begin();
	for (accountIt; accountIt != src->m_Accounts.end(); accountIt++)
	{
		BankAccount* account = *accountIt;
		AddAccount(account->GetId())->CopyFrom(account);
	}


	
	auto stockIt = src->m_Stocks.begin();
	for (stockIt; stockIt != src->m_Stocks.end(); stockIt++)
	{
		Stock* stock = *stockIt;
		AddStock(stock->GetId())->CopyFrom(stock);
	}

	
	auto cowClassIt = src->m_Classes.begin();
	for (cowClassIt; cowClassIt != src->m_Classes.end(); cowClassIt++)
	{
		Class* cowClass = *cowClassIt;
		AddClass(cowClass->GetId())->CopyFrom(cowClass);
	}
}

const IFarm* FarmOrganisation::GetFarm() const
{
	return m_pFarm;
}

Farm* FarmOrganisation::CreateFarm(uint32_t farmId)
{
	if(!m_pFarm)
		m_pFarm = new Farm(farmId);

	return m_pFarm;
}

PtrEnumerator<IHerd> FarmOrganisation::EnumHerds() const
{
	return PtrEnumerator<IHerd>(m_Herds);
}

PtrEnumerator<ISeason> FarmOrganisation::EnumSeasons() const
{
	return PtrEnumerator<ISeason>(m_Seasons);
}

PtrEnumerator<IAgent> FarmOrganisation::EnumAgents() const
{
	return PtrEnumerator<IAgent>(m_Agents);
}

PtrEnumerator<IBankAccount> FarmOrganisation::EnumAccounts() const
{
	return PtrEnumerator<IBankAccount>(m_Accounts);
}

Season* FarmOrganisation::AddSeason(uint32_t seasonId)
{
	Season* pNewSeason = new Season(seasonId);
	m_Seasons.Add(pNewSeason);
	return pNewSeason;
}

Herd* FarmOrganisation::AddHerd(uint32_t herdId)
{
	Herd* pNewHerd = new Herd(herdId);
	m_Herds.Add(pNewHerd);
	return pNewHerd;
}

Agent* FarmOrganisation::AddAgent(uint32_t agentId)
{
	Agent* agent = new Agent(agentId);
	m_Agents.Add(agent);
	return agent;
}

BankAccount* FarmOrganisation::AddAccount(uint32_t accountId)
{
	BankAccount* account = new BankAccount(accountId);
	m_Accounts.Add(account);
	return account;
}

PtrEnumerator<IClass> FarmOrganisation::EnumClasses() const
{
	return PtrEnumerator<IClass>(m_Classes);
}

PtrEnumerator<IStock> FarmOrganisation::EnumStocks() const
{
	return PtrEnumerator<IStock>(m_Stocks);
}

Class* FarmOrganisation::AddClass(uint32_t classId)
{
	Class* cowClass = new Class(classId);
	m_Classes.Add(cowClass);
	return cowClass;
}

Stock* FarmOrganisation::AddStock(uint32_t stockId)
{
	Stock* stock = new Stock(stockId);
	m_Stocks.Add(stock);
	return stock;
}

void FarmOrganisation::serialize(SF::Archive &archive)
{
	count_t herdCount, seasonCount, agentCount, accountCount, stockCount,classCount;
	if(archive.isWrite())
	{
		SerializableFarm(m_pFarm).serialize(archive);
		herdCount = m_Herds.size(); 
		archive & herdCount;
		auto herdIt = m_Herds.begin();
		for(herdIt;herdIt != m_Herds.end();herdIt++)
		{
			SerializableHerd(*herdIt).serialize(archive);
		}
		seasonCount = m_Seasons.size();
		archive & seasonCount;
		auto seasonIt = m_Seasons.begin();

		for(seasonIt; seasonIt != m_Seasons.end();seasonIt++)
		{
			SerializableSeason(*seasonIt).serialize(archive);
		}

		agentCount = m_Agents.size();
		archive & agentCount;
		auto agentIt = m_Agents.begin();

		for (agentIt; agentIt != m_Agents.end(); agentIt++)
		{
			SerializableAgent(*agentIt).serialize(archive);
		}

		accountCount = m_Accounts.size();
		archive & accountCount;
		auto accountIt = m_Accounts.begin();

		for (accountIt; accountIt != m_Accounts.end(); accountIt++)
		{
			BankAccountSerializer(*accountIt).serialize(archive);
		}


		stockCount = m_Stocks.size();
		archive & stockCount;
		auto stockIt = m_Stocks.begin();

		for (stockIt; stockIt != m_Stocks.end(); stockIt++)
		{
			SerializableStock(*stockIt).serialize(archive);
		}

		classCount = m_Classes.size();
		archive & classCount;
		auto cowClassIt = m_Classes.begin();

		for (cowClassIt; cowClassIt != m_Classes.end(); cowClassIt++)
		{
			SerializableClass(*cowClassIt).serialize(archive);
		}		
		
	}
	else
	{
		SerializableFarm(CreateFarm(NULL_ID)).serialize(archive);
		archive & herdCount;
		for (count_t herd = 0; herd < herdCount; herd++)
		{
			SerializableHerd(AddHerd(NULL_ID)).serialize(archive);
		}
		archive & seasonCount;
		for (count_t season = 0; season < seasonCount; season++)
		{
			SerializableSeason(AddSeason(NULL_ID)).serialize(archive);
		}
				
		archive & agentCount;
		for (count_t agent = 0; agent < agentCount; agent++)
		{
			SerializableAgent(AddAgent(NULL_ID)).serialize(archive);
		}

		archive & accountCount;
		for (count_t account = 0; account < accountCount; account++)
		{
			BankAccountSerializer(AddAccount(NULL_ID)).serialize(archive);
		}
		
		archive & stockCount;
		for (count_t stock = 0; stock < stockCount; stock++)
		{
			SerializableStock(AddStock(NULL_ID)).serialize(archive);
		}
		
		archive & classCount;
		for (count_t cowClass = 0; cowClass < classCount; cowClass++)
		{
			SerializableClass(AddClass(NULL_ID)).serialize(archive);
		}
	}
}


	
