#include "stdafx.h"
#include "CowTracker.h"
#include <arrays/DeepDelete.h>

CowTracker::~CowTracker()
{
	DeepDelete(m_trailsMap);
}

CowTrail* CowTracker::StartTrail(ICow* pCow)
{
	auto trailIt = m_trailsMap.find(pCow->GetId());

	if(trailIt != m_trailsMap.end())
	{
		return trailIt->second;
	}
	else
	{
		CowTrail* pNewTrail = new CowTrail(pCow);
		m_trailsMap[pCow->GetId()] = pNewTrail;
		return pNewTrail;
	}
}

CowTrail* CowTracker::GetTrail(uint32_t cowId)
{
	auto trailIt = m_trailsMap.find(cowId);

	if(trailIt != m_trailsMap.end())
	{
		return trailIt->second;
	}
	return NULL;
}

void CowTracker::DeleteTrail(uint32_t cowId)
{
	auto trailIt = m_trailsMap.find(cowId);

	if(trailIt != m_trailsMap.end())
	{
		delete trailIt->second;
		m_trailsMap.erase(trailIt);
	}
}
