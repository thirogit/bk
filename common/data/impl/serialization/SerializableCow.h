#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Cow.h"

class SerializableCow
{
public:
	SerializableCow(Cow* pCow);

	uint32_t&    Id();
	std::wstring& PassImgLabel();
	//Latitude& PositionLatitude();		
	//Longitude& PositionLongitude();	
	CowDetails& Details();
	uint32_t& BuyInvoiceId();
	uint32_t& SellInvoiceId();

	void serialize(SF::Archive &archive);

private:
	Cow* m_pCow;
};
