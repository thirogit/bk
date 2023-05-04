#include "stdafx.h"
#include "UniqueIds.h"
#include <boost\foreach.hpp>

UniqueIds::UniqueIds()
{
}

UniqueIds::UniqueIds(UniqueIds&& ids)
{
	BOOST_FOREACH(uint32_t id,ids.m_ids)
	{
		m_ids.insert(id);
	}

	ids.m_ids.clear();
}

UniqueIds& UniqueIds::operator=(const UniqueIds& right)
{
	m_ids.clear();
	BOOST_FOREACH(uint32_t id, right.m_ids)
	{
		m_ids.insert(id);
	}
	return *this;
}

UniqueIds::UniqueIds(const UniqueIds& ids)
{
	operator=(ids);
}

void UniqueIds::add(uint32_t id)
{
	m_ids.insert(id);
}

void UniqueIds::remove(uint32_t id)
{
	m_ids.erase(id);
}

UniqueIds::const_iterator UniqueIds::begin() const
{
	return m_ids.cbegin();
}

UniqueIds::const_iterator UniqueIds::end() const
{
	return m_ids.cend();
}

UniqueIds::iterator UniqueIds::begin()
{
	return m_ids.begin();
}

UniqueIds::iterator UniqueIds::end()
{
	return m_ids.end();
}

void UniqueIds::clear()
{
	m_ids.clear();
}