#include "stdafx.h"
#include "FetchHentsResponse.h"
#include <data/impl/serialization/SerializableHent.h>

FetchHentsResponse::FetchHentsResponse() 
{
}

FetchHentsResponse::~FetchHentsResponse()
{
		m_Hents.FreeAndFlush();
}	

HentWireObject* FetchHentsResponse::AddHent(uint32_t hentId)
{
	HentWireObject* pNewHent = new HentWireObject(hentId);
	m_Hents.Add(pNewHent);
	return pNewHent;
}
		
PtrEnumerator<const HentWireObject> FetchHentsResponse::GetHents()
{
	return PtrEnumerator<const HentWireObject>(m_Hents);
}

count_t FetchHentsResponse::GetHentsCount() const
{
	return m_Hents.size();
}

void FetchHentsResponse::serialize(SF::Archive &archive)
{
	count_t hentCount;
	HentWireObject* pHentObj;

	if(archive.isRead())
	{
		archive & hentCount;		
		bool bLocked;
		for(count_t i = 0;i < hentCount;i++)
		{
			pHentObj = AddHent(NULL_ID);
			archive & bLocked;
			pHentObj->SetIsLocked(bLocked);
			SerializableHent(pHentObj).serialize(archive);
		}
	}	
	else
	{
		hentCount = m_Hents.size();
		archive & hentCount;
		for(count_t i = 0;i < hentCount;i++)
		{
			pHentObj = m_Hents[i];
			archive & pHentObj->GetIsLocked();
			SerializableHent(m_Hents[i]).serialize(archive);
		}
	}
}
