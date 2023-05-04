#include "stdafx.h"
#include "HerdAudience.h"
#include <algorithm>
#include <boost\range.hpp>

void HerdAudience::AddObserver(uint32_t herdId,HerdObserver* pObserver)
{
	boost::iterator_range<std::multimap<uint32_t,HerdObserver*>::iterator> observersRange = m_HerdObservers.equal_range(herdId);

	auto observerIt = std::find_if(observersRange.begin(), observersRange.end(), 
									[pObserver](std::pair<uint32_t,HerdObserver*> pair) 
									{ 
										return pObserver == pair.second; 
									});

	if(observerIt == observersRange.end())
	{
		m_HerdObservers.insert(std::pair<uint32_t,HerdObserver*>(herdId,pObserver));
	}
}

void HerdAudience::RemoveObserver(uint32_t herdId,HerdObserver* pObserver)
{
	auto observersRange = m_HerdObservers.equal_range(herdId);
	auto observerIt = std::find_if(observersRange.first, observersRange.second, 
									[pObserver](std::pair<uint32_t,HerdObserver*> pair) 
									{ 
										return pObserver == pair.second; 
									});
	if(observerIt != observersRange.second) 
		m_HerdObservers.erase(observerIt);	
}
