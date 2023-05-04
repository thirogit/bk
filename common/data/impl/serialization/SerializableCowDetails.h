#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../CowDetails.h"

class SerializableCowDetails
{
public:
	SerializableCowDetails(CowDetails& details);

	Decimal& Weight();
	uint32_t& StockId();	
	uint32_t&  ClassId();	
	CowNo& CowNumber();	
	CowNo& MotherNo();
	DateTime& BirthDate();
	std::wstring& PassNo();
	std::wstring& HealthCertNo();
	DateTime& PassDate();	
	std::wstring& BirthPlace();
	std::wstring& Extras();	
	uint32_t& FirstOwnerId();
	CowSex&	Sex();	
	NullDecimal& TermBuyPrice();
	NullDecimal& TermSellPrice();
	NullDecimal& TermBuyWeight();
	NullDecimal& TermSellWeight();
	uint32_t& TermBuyClassId();
	uint32_t& TermSellClassId();
	
	
	void serialize(SF::Archive &archive);

private:
	CowDetails& m_details;
};
