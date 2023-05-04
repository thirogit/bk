#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../InDoc.h"

class SerializableInDoc
{
public:
	SerializableInDoc(InDoc* pInDoc);	
	void serialize(SF::Archive &archive);
protected:
	DocDetails& Details();	
	DocContent& Content();	
	uint32_t& Id();
	uint32_t& HerdId();

private:
	InDoc* m_pInDoc;
};
