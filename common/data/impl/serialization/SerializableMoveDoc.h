#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../MoveDoc.h"

class SerializableMoveDoc
{
public:
	SerializableMoveDoc(MoveDoc* pDoc);	
	void serialize(SF::Archive &archive);
protected:
	DocDetails& Details();	
	DocContent& Content();	
	uint32_t& Id();
	uint32_t& HerdFromId();
	uint32_t& HerdToId();

private:
	MoveDoc* m_pDoc;
};
