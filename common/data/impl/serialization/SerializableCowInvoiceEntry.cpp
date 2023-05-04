#include "stdafx.h"
#include "SerializableCowInvoiceEntry.h"
#include "../../types/serialization/TypesSerialization.h"
#include "DataSerialization.h"

SerializableCowInvoiceEntry::SerializableCowInvoiceEntry(CowInvoiceEntry* pCowEntry) : m_pCowEntry(pCowEntry)
{
}

uint32_t& SerializableCowInvoiceEntry::CowId()
{
	return m_pCowEntry->m_cowId;
}

CowInvoiceEntryDetails& SerializableCowInvoiceEntry::Details()
{
	return m_pCowEntry->m_details;
}
	
void SerializableCowInvoiceEntry::serialize(SF::Archive &archive)
{
	archive & CowId();
	archive & Details();	
}