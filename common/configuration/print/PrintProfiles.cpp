#include "stdafx.h"
#include "PrintProfiles.h"
#include "../../arrays/DeepDelete.h"
#include "../../CommonTypes.h"
#include <boost/foreach.hpp>

PrintProfiles::PrintProfiles()
{
}

PrintProfiles::PrintProfiles(PrintProfiles&& profiles)
{
	std::copy(profiles.m_profiles.begin(), profiles.m_profiles.end(), std::back_inserter(m_profiles));
	profiles.m_profiles.clear();
}

PrintProfiles::PrintProfiles(const PrintProfiles& profiles)
{
	BOOST_FOREACH(PrintProfile* profile, profiles)
	{
		AddProfile(profile);
	}
}

PrintProfiles::~PrintProfiles()
{
	DeepDelete(m_profiles);
	m_profiles.clear();
}

void PrintProfiles::AddProfile(const IPrintProfile* profile)
{
	m_profiles.push_back(new PrintProfile(profile));
}

PtrEnumerator<IPrintProfile> PrintProfiles::EnumProfiles()
{
	return PtrEnumerator<IPrintProfile>(m_profiles);
}

PrintProfiles::const_iterator PrintProfiles::begin() const
{
	return m_profiles.begin();
}

PrintProfiles::const_iterator PrintProfiles::end() const
{
	return m_profiles.end();
}

PrintProfiles::iterator PrintProfiles::begin()
{
	return m_profiles.begin();
}

PrintProfiles::iterator PrintProfiles::end()
{
	return m_profiles.end();
}