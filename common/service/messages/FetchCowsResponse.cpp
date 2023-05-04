#include "stdafx.h"
#include "FetchCowsResponse.h"
#include <data/impl/serialization/SerializableCow.h>

FetchCowsResponse::FetchCowsResponse() 
{
}

FetchCowsResponse::~FetchCowsResponse()
{
	m_Cows.FreeAndFlush();
}	

CowWireObject* FetchCowsResponse::AddCow(uint32_t cowId)
{
	CowWireObject* pNewCow = new CowWireObject(cowId);
	m_Cows.Add(pNewCow);
	return pNewCow;
}
		
PtrEnumerator<const CowWireObject> FetchCowsResponse::GetCows()
{
	return PtrEnumerator<const CowWireObject>(m_Cows);
}

count_t FetchCowsResponse::GetCowsCount() const
{
	return m_Cows.size();
}

void FetchCowsResponse::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			AddCow(NULL_ID)->serialize(archive);
		}
	}	
	else
	{
		count = m_Cows.size();
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			m_Cows[i]->serialize(archive);
		}
	}
}
