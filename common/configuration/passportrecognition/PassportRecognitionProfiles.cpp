#include "stdafx.h"
#include "PassportRecognitionProfiles.h"
#include "../../arrays/DeepDelete.h"
#include "../../CommonTypes.h"
#include <boost/foreach.hpp>

PassportRecognitionProfiles::PassportRecognitionProfiles()
{
}

PassportRecognitionProfiles::PassportRecognitionProfiles(PassportRecognitionProfiles&& profiles)
{
	std::copy(profiles.m_profiles.begin(), profiles.m_profiles.end(), std::back_inserter(m_profiles));
	profiles.m_profiles.clear();
}

PassportRecognitionProfiles::PassportRecognitionProfiles(const PassportRecognitionProfiles& profiles)
{
	BOOST_FOREACH(PassportRecognitionProfile* profile, profiles)
	{
		AddProfile(profile);
	}
}

PassportRecognitionProfiles::~PassportRecognitionProfiles()
{
	DeepDelete(m_profiles);
	m_profiles.clear();
}

void PassportRecognitionProfiles::AddProfile(const IPassportRecognitionProfile* profile)
{
	m_profiles.push_back(new PassportRecognitionProfile(profile));
}

PtrEnumerator<IPassportRecognitionProfile> PassportRecognitionProfiles::EnumProfiles()
{
	return PtrEnumerator<IPassportRecognitionProfile>(m_profiles);
}

PassportRecognitionProfiles::const_iterator PassportRecognitionProfiles::begin() const
{
	return m_profiles.begin();
}

PassportRecognitionProfiles::const_iterator PassportRecognitionProfiles::end() const
{
	return m_profiles.end();
}

PassportRecognitionProfiles::iterator PassportRecognitionProfiles::begin()
{
	return m_profiles.begin();
}

PassportRecognitionProfiles::iterator PassportRecognitionProfiles::end()
{
	return m_profiles.end();
}