#include "stdafx.h"
#include "CowPresenceKeys.h"
#include <iterator>

CowPresenceKeys::CowPresenceKeys()
{

}

CowPresenceKeys::~CowPresenceKeys()
{
}

CowPresenceKeys::CowPresenceKeys(const CowPresenceKeys& src)
{
	std::copy(src.begin(), src.end(), std::inserter(m_keys,m_keys.begin()));
}

CowPresenceKeys::CowPresenceKeys(CowPresenceKeys&& src)
{
	m_keys = std::move(src.m_keys);
}
		
void CowPresenceKeys::add(const CowPresenceKey& key)
{
	m_keys.insert(key);
}

CowPresenceKeys::const_iterator CowPresenceKeys::begin() const
{
	return m_keys.cbegin();
}

CowPresenceKeys::const_iterator CowPresenceKeys::end() const
{
	return m_keys.cend();
}

CowPresenceKeys::iterator CowPresenceKeys::begin()
{
	return m_keys.begin();
}

CowPresenceKeys::iterator CowPresenceKeys::end()
{
	return m_keys.end();
}




