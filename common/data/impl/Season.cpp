#include "stdafx.h"
#include "Season.h"

Season::Season(uint32_t id) : m_Id(id)
{
}

Season::Season(const ISeason* pSrc) : m_Id(pSrc->GetId())
{
	CopyFrom(pSrc);
}

const std::wstring& Season::GetSeasonName() const
{
	return m_SeasonName;
}

const DateTime&	Season::GetStartDate() const
{
	return m_StartDate;
}

const DateTime&	Season::GetEndDate() const
{
	return m_EndDate;
}
		
uint32_t Season::GetId() const
{
	return m_Id;
}


void Season::SetSeasonName(const std::wstring& sSeasonName)
{
	m_SeasonName = sSeasonName;
}

void Season::SetStartDate(const DateTime& startDate)
{
	m_StartDate = startDate;
}

void Season::SetEndDate(const DateTime& endDate)
{
	m_EndDate = endDate;
}
		
void Season::CopyFrom(const ISeason* pSrc)
{	
	SetSeasonName(pSrc->GetSeasonName());
	SetStartDate(pSrc->GetStartDate());
	SetEndDate(pSrc->GetEndDate());
}
		
		
