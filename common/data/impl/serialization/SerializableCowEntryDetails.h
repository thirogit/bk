#pragma once 

#include <SF/Archive.hpp>
#include "../CowEntryDetails.h"

class SerializableCowEntryDetails
{
public:
	SerializableCowEntryDetails(CowEntryDetails& details);	
	void serialize(SF::Archive &archive);
protected:
	NullDecimal& Weight();	
	uint32_t& StockId();	
	uint32_t& ClassId();			
	NullInt& Group();	
	
private:
	CowEntryDetails& m_details;
};
