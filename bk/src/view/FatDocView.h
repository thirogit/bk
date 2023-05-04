#ifndef __FATDOCVIEW_H__
#define __FATDOCVIEW_H__

#include <stdint.h>
#include "CowPresenceView.h"
#include <vector>
#include <arrays\DeepDelete.h>

template<typename DOCVIEW>
class FatDocView : public DOCVIEW
{
public:
	FatDocView(uint32_t docId) : DOCVIEW(docId)
	{	
	}

	FatDocView(const FatDocView& src) : DOCVIEW(src)
	{	
		auto it = src.m_cows.begin();
		while(it != src.m_cows.end())
		{
			m_cows.push_back(new CowPresenceView(*(*it)));
			it++;
		}
	}

	~FatDocView()
	{
		DeepDelete(m_cows);
	}
		
	CowPresenceView* AddCow(CowPresenceKey& key)
	{
		CowPresenceView* pView = new CowPresenceView(key);
		m_cows.push_back(pView);
		return pView;
	}
	
	size_t GetCowCount() const	
	{
		return m_cows.size();
	}
	
	const CowPresenceView* GetCowAt(int index) const
	{
		if(index >= 0 && index < m_cows.size())
			return m_cows[index];
			
		return NULL;
	}
	
private:
	
	std::vector<CowPresenceView*> m_cows;
	
};

#endif