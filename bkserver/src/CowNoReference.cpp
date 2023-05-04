#include "stdafx.h"
#include "CowNoReference.h"
#include <boost/iterator/transform_iterator.hpp>
#include <boost/function.hpp>

void CowNoReference::AddReference(const CowNo& cowNo,uint32_t cowId)
{
	m_References.insert(std::pair<CowNo,uint32_t>(cowNo,cowId));
}

void CowNoReference::RemoveReference(const CowNo& cowNo,uint32_t cowId)
{
	auto range = m_References.equal_range(cowNo);
	auto elementIt = std::find_if(range.first,range.second,[cowId](std::pair<CowNo,uint32_t> element) -> bool
														{
															return element.second == cowId;
														});

	if(elementIt != range.second)
		m_References.erase(elementIt);
}
	
CowIds CowNoReference::GetCowsWithCowNo(const CowNo& cowNo)
{
	auto range = m_References.equal_range(cowNo);

	boost::transform_iterator<boost::function<uint32_t (std::pair<CowNo,uint32_t>)>,std::multimap<CowNo,uint32_t>::iterator> it,endIt;
	boost::function<uint32_t (std::pair<CowNo,uint32_t>)> f =  [](std::pair<CowNo,uint32_t> pair) -> uint32_t { return pair.second; };
	
	it = boost::make_transform_iterator(range.first, f);
	endIt = boost::make_transform_iterator(range.second, f);
	

	return CowIds(it,endIt);
}

