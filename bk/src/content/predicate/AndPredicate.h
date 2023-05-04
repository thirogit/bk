#pragma once

#include "Predicate.h"
#include <vector>
#include <boost\foreach.hpp>

template<class T>
class AndPredicate : public Predicate<T>
{
public:

	AndPredicate(const std::vector<Predicate<T>*>& predicates)
	{
		std::copy(predicates.begin(), predicates.end(), std::back_inserter(m_predicates));
	}

	virtual bool Test(T* object)
	{
		BOOST_FOREACH(Predicate<T>* predicate, m_predicates)
		{
			if (!predicate->Test(object))
			{
				return false;
			}
		}

		return true;
	}
private:
	std::vector<Predicate<T>*> m_predicates;

};
