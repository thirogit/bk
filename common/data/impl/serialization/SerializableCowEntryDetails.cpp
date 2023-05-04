#include "stdafx.h"
#include "SerializableCowEntryDetails.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableCowEntryDetails::SerializableCowEntryDetails(CowEntryDetails& details) : m_details(details)
{
}

NullDecimal& SerializableCowEntryDetails::Weight()
{
	return m_details.m_weight;
}

uint32_t& SerializableCowEntryDetails::StockId()
{
	return m_details.m_stockId;
}

uint32_t& SerializableCowEntryDetails::ClassId()
{
	return m_details.m_classId;
}

NullInt& SerializableCowEntryDetails::Group()
{
	return m_details.m_group;
}
	
void SerializableCowEntryDetails::serialize(SF::Archive &archive)
{
	archive & Weight();	
	archive & StockId();	
	archive & ClassId();			
	archive & Group();	
}