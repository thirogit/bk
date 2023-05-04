#ifndef __OBSERVERABLE_H__
#define __OBSERVERABLE_H__

#include <set>
#include <algorithm>

template<class OBSERVER>
class Observerable
{
public:
	void AddObserver(OBSERVER* pObserver)
	{
		m_Observers.insert(pObserver);
	}
	
	void RemoveObserver(OBSERVER* pObserver)
	{
		m_Observers.erase(pObserver);
	}

	template<typename F> void VisitObservers(F& f)
	{
		std::function<void (OBSERVER*)> visitor = f;
		std::for_each(m_Observers.begin(),m_Observers.end(),visitor);
	}

	template<typename M,typename A1> void VisitObservers(M facadeMem,A1 a1)
	{
		auto facadeFn = std::mem_fn(facadeMem);
		std::function<void (OBSERVER*)> visitor = std::bind(facadeFn,std::placeholders::_1,a1);
		std::for_each(m_Observers.begin(),m_Observers.end(),visitor);
	}

	void RemoveAllObservers()
	{
		m_Observers.clear();
	}

private:
	std::set<OBSERVER*> m_Observers;
};

#endif