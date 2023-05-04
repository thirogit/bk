#include "stdafx.h"
#include "UniquePresenceKeys.h"
#include <boost\foreach.hpp>

UniquePresenceKeys::UniquePresenceKeys()
{
}

UniquePresenceKeys::UniquePresenceKeys(UniquePresenceKeys&& keys)
{
	BOOST_FOREACH(CowPresenceKey key,keys.m_keys)
	{
		m_keys.insert(key);
	}

	keys.m_keys.clear();
}

UniquePresenceKeys::UniquePresenceKeys(const UniquePresenceKeys& keys)
{
	BOOST_FOREACH(CowPresenceKey key,keys.m_keys)
	{
		m_keys.insert(key);
	}
}

void UniquePresenceKeys::add(const CowPresenceKey& key)
{
	m_keys.insert(key);
}

void UniquePresenceKeys::remove(const CowPresenceKey& key)
{
	m_keys.erase(key);
}

UniquePresenceKeys::const_iterator UniquePresenceKeys::begin() const
{
	return m_keys.cbegin();
}

UniquePresenceKeys::const_iterator UniquePresenceKeys::end() const
{
	return m_keys.cend();
}

UniquePresenceKeys::iterator UniquePresenceKeys::begin()
{
	return m_keys.begin();
}

UniquePresenceKeys::iterator UniquePresenceKeys::end()
{
	return m_keys.end();
}

