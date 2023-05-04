#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Season.h"

class SerializableSeason
{
public:
	SerializableSeason(Season* pSeason);	
		
	std::wstring& SeasonName();
	DateTime& StartDate();
	DateTime& EndDate();
	uint32_t& Id();

	void serialize(SF::Archive &archive);
private:
	Season* m_pSeason;
};
