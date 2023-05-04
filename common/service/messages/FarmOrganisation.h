#ifndef __FARMORGANISATION_H__
#define __FARMORGANISATION_H__

#include <data/impl/Farm.h>
#include <data/impl/Herd.h>
#include <data/impl/Season.h>
#include <data/impl/Agent.h>
#include <data/impl/Stock.h>
#include <data/impl/Class.h>
#include <data/impl/BankAccount.h>
#include <arrays/NewPtrFlushArray.h>
#include <SF/Archive.hpp>
#include <arrays/Enumerator.h>

class FarmOrganisation
{
public:
	FarmOrganisation();
	~FarmOrganisation();

	void CopyFrom(const FarmOrganisation* src);

	const IFarm* GetFarm() const;
	PtrEnumerator<IHerd> EnumHerds() const;
	PtrEnumerator<ISeason> EnumSeasons() const;
	PtrEnumerator<IAgent> EnumAgents() const;
	PtrEnumerator<IBankAccount> EnumAccounts() const;
	PtrEnumerator<IClass> EnumClasses() const;
	PtrEnumerator<IStock> EnumStocks() const;

	Season* AddSeason(uint32_t seasonId);
	Herd* AddHerd(uint32_t herdId);
	Agent* AddAgent(uint32_t agentId);
	BankAccount* AddAccount(uint32_t accountId);
	Class* AddClass(uint32_t classId);
	Stock* AddStock(uint32_t stockId);

	Farm* CreateFarm(uint32_t farmId);	
	void serialize(SF::Archive &archive);
private:
	void Destroy();
private:
	Farm* m_pFarm;
	NewPtrFlushArray<Season> m_Seasons;
	NewPtrFlushArray<Herd> m_Herds;
	NewPtrFlushArray<Agent> m_Agents;
	NewPtrFlushArray<BankAccount> m_Accounts;
	NewPtrFlushArray<Stock> m_Stocks;
	NewPtrFlushArray<Class> m_Classes;
	
};

#endif

