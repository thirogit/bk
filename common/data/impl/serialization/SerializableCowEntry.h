#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../CowEntry.h"

class SerializableCowEntry
{
public:
	SerializableCowEntry(CowEntry* pCowEntry);	
	void serialize(SF::Archive &archive);
protected:
	uint32_t& CowId();
	CowEntryDetails& Details();
	
private:
	CowEntry* m_pCowEntry;
};
