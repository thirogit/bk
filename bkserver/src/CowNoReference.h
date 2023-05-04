#pragma once

#include <boost/range/any_range.hpp>
#include <data/types/CowNo.h>
#include <map>

typedef boost::any_range<uint32_t,boost::forward_traversal_tag, uint32_t, std::ptrdiff_t> CowIds;

class CowNoReference
{
public:
	void AddReference(const CowNo& cowNo,uint32_t cowId);
	void RemoveReference(const CowNo& cowNo,uint32_t cowId);
	
	CowIds GetCowsWithCowNo(const CowNo& cowNo);

private:
	std::multimap<CowNo,uint32_t> m_References;
};