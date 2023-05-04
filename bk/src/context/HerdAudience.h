#ifndef __HERDAUDIENCE_H__
#define __HERDAUDIENCE_H__

#include "HerdObserver.h"
#include <map>

class HerdAudience 
{		
public:
	void AddObserver(uint32_t herdId,HerdObserver* pObserver);
	void RemoveObserver(uint32_t herdId,HerdObserver* pObserver);

	template<typename F> void VisitObservers(uint32_t herdId,F f)
	{
		std::function<void (HerdObserver*)> visitor = f;

		std::function<void (std::pair<uint32_t,HerdObserver*> pair)> invokeObserver = 
			[&visitor](std::pair<uint32_t,HerdObserver*> pair)
		{
			visitor(pair.second);
		};

		auto observersRange = m_HerdObservers.equal_range(herdId);
		std::for_each(observersRange.first,observersRange.second,invokeObserver);
	}
	
private:

	std::multimap<uint32_t,HerdObserver*> m_HerdObservers;


};


#endif