#include "stdafx.h"
#include "FetchPrintProfilesResponse.h"
#include "../../data/IIdable.h"
#include "../../configuration/serialization/SerializablePrintProfile.h"

FetchPrintProfilesResponse::FetchPrintProfilesResponse() 
{
}

FetchPrintProfilesResponse::~FetchPrintProfilesResponse()
{
	m_Profiles.FreeAndFlush();
}	

PrintProfile* FetchPrintProfilesResponse::AddProfile(uint32_t profileId)
{
	PrintProfile* pNew = new PrintProfile(profileId);
	m_Profiles.Add(pNew);
	return pNew;
}
		
PtrEnumerator<IPrintProfile> FetchPrintProfilesResponse::GetProfiles()
{
	return PtrEnumerator<IPrintProfile>(m_Profiles);
}

count_t FetchPrintProfilesResponse::GetProfilesCount() const
{
	return m_Profiles.size();
}

void FetchPrintProfilesResponse::serialize(SF::Archive &archive)
{
	count_t profileCount;
	PrintProfile* pProfileObj;

	if(archive.isRead())
	{
		archive & profileCount;
		for (count_t i = 0; i < profileCount; i++)
		{
			pProfileObj = AddProfile(NULL_ID);
			SerializablePrintProfile(pProfileObj).serialize(archive);
		}
	}	
	else
	{
		profileCount = m_Profiles.size();
		archive & profileCount;
		for (count_t i = 0; i < profileCount; i++)
		{
			pProfileObj = m_Profiles[i];
			SerializablePrintProfile(pProfileObj).serialize(archive);
		}
	}
}
