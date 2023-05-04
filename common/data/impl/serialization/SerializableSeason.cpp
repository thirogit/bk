#include "stdafx.h"
#include "SerializableSeason.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableSeason::SerializableSeason(Season* pSeason) : m_pSeason(pSeason)
{

}

std::wstring& SerializableSeason::SeasonName()
{
	return m_pSeason->m_SeasonName;
}

DateTime& SerializableSeason::StartDate()
{
	return m_pSeason->m_StartDate;
}

DateTime& SerializableSeason::EndDate()
{
	return m_pSeason->m_EndDate;
}

uint32_t& SerializableSeason::Id()
{
	return m_pSeason->m_Id;
}

void SerializableSeason::serialize(SF::Archive &archive)
{
	archive & Id() & SeasonName() & StartDate() & EndDate();
}