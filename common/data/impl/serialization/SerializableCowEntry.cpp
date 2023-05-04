#include "stdafx.h"
#include "SerializableCowEntry.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableCowEntryDetails.h"

SerializableCowEntry::SerializableCowEntry(CowEntry* pCowEntry) : m_pCowEntry(pCowEntry)
{
}

uint32_t& SerializableCowEntry::CowId()
{
	return m_pCowEntry->m_cowId;
}

CowEntryDetails& SerializableCowEntry::Details()
{
	return m_pCowEntry->m_details;
}
	
void SerializableCowEntry::serialize(SF::Archive &archive)
{
	archive & CowId();
	SerializableCowEntryDetails(Details()).serialize(archive);	
}