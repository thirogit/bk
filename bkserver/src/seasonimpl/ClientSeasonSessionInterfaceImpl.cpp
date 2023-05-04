#include "stdafx.h"
#include "ClientSeasonSessionInterfaceImpl.h"
#include "SeasonSession.h"
#include "FarmSession.h"

ClientSeasonSessionInterfaceImpl::ClientSeasonSessionInterfaceImpl(SeasonSession* pSession) : m_pSession(pSession)
{
}

void ClientSeasonSessionInterfaceImpl::Close(ClientSeasonSession* pSession)
{
	DoAny(pSession,[this,pSession](SeasonSessionFacade* facade)
	{
		m_pSession->Close(pSession);
	});
}

ISeason* ClientSeasonSessionInterfaceImpl::GetSeason() const
{
	return m_pSession->GetSeason();
}

IFarm* ClientSeasonSessionInterfaceImpl::GetFarm() const
{
	return m_pSession->m_pFarmSession->GetFarm();
}

IClass* ClientSeasonSessionInterfaceImpl::FindClass(uint32_t classId)
{
	return m_pSession->FindClass(classId);
}

IStock* ClientSeasonSessionInterfaceImpl::FindStock(uint32_t stockId)
{
	return m_pSession->FindStock(stockId);
}

IHerd* ClientSeasonSessionInterfaceImpl::FindHerd(uint32_t herdId)
{
	return m_pSession->FindHerdById(herdId);
}

IAgent* ClientSeasonSessionInterfaceImpl::FindAgent(uint32_t agentId)
{
	return m_pSession->FindAgent(agentId);
}

uint16_t ClientSeasonSessionInterfaceImpl::GetObserverPort() const
{
	return m_pSession->GetObserverPort();
}

FetchHandle ClientSeasonSessionInterfaceImpl::OpenFetchHandle()
{
	return m_pSession->OpenFetchHandle();
}

void ClientSeasonSessionInterfaceImpl::Do(ClientSeasonSession* pClient,std::function<void (SeasonSessionFacade*)>& action)
{
	m_pSession->Do(pClient,action);
}
