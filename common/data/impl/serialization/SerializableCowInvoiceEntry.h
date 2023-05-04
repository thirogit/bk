#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../CowInvoiceEntry.h"

class SerializableCowInvoiceEntry
{
public:
	SerializableCowInvoiceEntry(CowInvoiceEntry* pCowEntry);	
	void serialize(SF::Archive &archive);
protected:
	uint32_t& CowId();
	CowInvoiceEntryDetails& Details();
	
private:
	CowInvoiceEntry* m_pCowEntry;
};
