#pragma once

#include <algorithm>

template<typename Value>
class Diffrence
{
public:
	typedef boost::any_range<Value,boost::forward_traversal_tag, Value, std::ptrdiff_t> Range;

	Diffrence(const std::vector<Value>& oldValues,const std::vector<Value>& newValues)
	{
		std::set<Value> _newValues;
		std::copy(newValues.begin(),newValues.end(),std::inserter(_newValues,_newValues.begin()));
		
		std::set<Value> _oldValues;
		std::copy(oldValues.begin(),oldValues.end(),std::inserter(_oldValues,_oldValues.begin()));
		
		std::set_difference(_newValues.begin(), _newValues.end(),
							_oldValues.begin(), _oldValues.end(),
						   std::back_inserter(m_addedValues));						   
						   
		std::set_difference(_oldValues.begin(), _oldValues.end(),
			_newValues.begin(), _newValues.end(),
						   std::back_inserter(m_removedValues));
	
		std::set_intersection(_oldValues.begin(), _oldValues.end(),
							_newValues.begin(), _newValues.end(),
						   std::back_inserter(m_commonValues));
		
		
	}

	Range Added()
	{
		return Range(m_addedValues);
	}

	Range Removed()
	{
		return Range(m_removedValues);
	}

	Range Common()
	{
		return Range(m_commonValues);
	}
	
	
	
private:
	std::vector<Value> m_addedValues;
	std::vector<Value> m_removedValues;
	std::vector<Value> m_commonValues;
	
};

