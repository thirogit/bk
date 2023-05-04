#include "stdafx.h"
#include "FetchPassportRecognitionProfilesResponse.h"
#include "../../data/IIdable.h"
#include "../../configuration/serialization/SerializablePassportRecognitionProfile.h"

FetchPassportRecognitionProfilesResponse::FetchPassportRecognitionProfilesResponse() 
{
}

FetchPassportRecognitionProfilesResponse::~FetchPassportRecognitionProfilesResponse()
{
	m_Profiles.FreeAndFlush();
}	

PassportRecognitionProfile* FetchPassportRecognitionProfilesResponse::AddProfile(uint32_t profileId)
{
	PassportRecognitionProfile* pNew = new PassportRecognitionProfile(profileId);
	m_Profiles.Add(pNew);
	return pNew;
}
		
PtrEnumerator<IPassportRecognitionProfile> FetchPassportRecognitionProfilesResponse::GetProfiles()
{
	return PtrEnumerator<IPassportRecognitionProfile>(m_Profiles);
}

count_t FetchPassportRecognitionProfilesResponse::GetProfilesCount() const
{
	return m_Profiles.size();
}

void FetchPassportRecognitionProfilesResponse::serialize(SF::Archive &archive)
{
	count_t profileCount;
	PassportRecognitionProfile* pProfileObj;

	if(archive.isRead())
	{
		archive & profileCount;
		for (count_t i = 0; i < profileCount; i++)
		{
			pProfileObj = AddProfile(NULL_ID);
			SerializablePassportRecognitionProfile(pProfileObj).serialize(archive);
		}
	}	
	else
	{
		profileCount = m_Profiles.size();
		archive & profileCount;
		for (count_t i = 0; i < profileCount; i++)
		{
			pProfileObj = m_Profiles[i];
			SerializablePassportRecognitionProfile(pProfileObj).serialize(archive);
		}
	}
}
