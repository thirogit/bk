#include "stdafx.h"
#include "PassportRecognitionProfileRequest.h"
#include "../../configuration/serialization/SerializablePassportRecognitionProfile.h"
#include "../../data/IIdable.h"


PassportRecognitionProfileRequest::PassportRecognitionProfileRequest() : m_pProfile(NULL)
{
}

PassportRecognitionProfileRequest::~PassportRecognitionProfileRequest()
{
	if (m_pProfile)
		delete m_pProfile;
}

PassportRecognitionProfile* PassportRecognitionProfileRequest::GetProfile() const
{
	return m_pProfile;
}

PassportRecognitionProfile* PassportRecognitionProfileRequest::CreateProfile(uint32_t profileId)
{
	if (!m_pProfile)
		m_pProfile = new PassportRecognitionProfile(profileId);
	return m_pProfile;
}

void PassportRecognitionProfileRequest::serialize(SF::Archive &archive)
{
	if(archive.isRead())
	{
		CreateProfile(NULL_ID);
		SerializablePassportRecognitionProfile(m_pProfile).serialize(archive);
	}
	else
	{
		SerializablePassportRecognitionProfile(m_pProfile).serialize(archive);
	}
}
	
