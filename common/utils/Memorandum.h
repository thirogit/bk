#pragma once

#include <boost/foreach.hpp>
#include <vector>
#include <functional>

class Memorandum
{
	typedef std::vector< std::function<void ()> > undos_vector;

public:
	template<typename F>
	void Add(F const & f)
	{
		std::function<void ()> _f = f;
		m_undos.push_back(_f);
	}

	void Clear()
	{
		m_undos.clear();
	}	
	
	void Do()
	{
		undos_vector::iterator it = m_undos.begin();
		while(it != m_undos.end())
		{
			(*it)();
			it++;
		}	
	}
private:
	undos_vector m_undos;

};