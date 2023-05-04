#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../OutDoc.h"

class SerializableOutDoc
{
public:
	SerializableOutDoc(OutDoc* pDoc);	
	void serialize(SF::Archive &archive);
protected:
	DocDetails& Details();	
	DocContent& Content();	
	uint32_t& Id();
	uint32_t& HerdId();

private:
	OutDoc* m_pDoc;
};
