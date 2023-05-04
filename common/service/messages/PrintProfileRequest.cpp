#include "stdafx.h"
#include "PrintProfileRequest.h"
#include "../../configuration/serialization/SerializablePrintProfile.h"
#include "../../data/IIdable.h"


PrintProfileRequest::PrintProfileRequest() : m_pProfile(NULL)
{
}

PrintProfileRequest::~PrintProfileRequest()
{
	if (m_pProfile)
		delete m_pProfile;
}

PrintProfile* PrintProfileRequest::GetProfile() const
{
	return m_pProfile;
}

PrintProfile* PrintProfileRequest::CreateProfile(uint32_t profileId)
{
	if (!m_pProfile)
		m_pProfile = new PrintProfile(profileId);
	return m_pProfile;
}

void PrintProfileRequest::serialize(SF::Archive &archive)
{
	if(archive.isRead())
	{
		CreateProfile(NULL_ID);
		SerializablePrintProfile(m_pProfile).serialize(archive);
	}
	else
	{
		SerializablePrintProfile(m_pProfile).serialize(archive);
	}
}
	
