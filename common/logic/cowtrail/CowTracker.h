#pragma once

#include "CowTrail.h"
#include <data/ICow.h>
#include <map>

class CowTracker
{
public:
	~CowTracker();
	CowTrail* StartTrail(ICow* pCow);
	CowTrail* GetTrail(uint32_t cowId);
	void DeleteTrail(uint32_t cowId);
private:	
	std::map<uint32_t,CowTrail*> m_trailsMap;

};
