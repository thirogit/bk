#pragma once

#include "ClientSeasonSessionInterface.h"

class SeasonSession;
class ClientSeasonSession;
	
class ClientSeasonSessionInterfaceImpl : public ClientSeasonSessionInterface
{
public:
	ClientSeasonSessionInterfaceImpl(SeasonSession* pSession);

	virtual void Close(ClientSeasonSession* pSession);
	virtual ISeason* GetSeason() const;
	virtual IFarm* GetFarm() const;
	virtual IClass* FindClass(uint32_t classId);
	virtual IStock* FindStock(uint32_t stockId);
	virtual IHerd* FindHerd(uint32_t herdId);		
	virtual IAgent* FindAgent(uint32_t agentId);		
	virtual uint16_t GetObserverPort() const;
	virtual FetchHandle OpenFetchHandle();		
	virtual void Do(ClientSeasonSession* pClient,std::function<void (SeasonSessionFacade*)>& action);

private:
	SeasonSession* m_pSession;
};
		
