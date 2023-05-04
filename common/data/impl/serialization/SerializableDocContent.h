#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../DocContent.h"

class SerializableDocContent
{
public:
	SerializableDocContent(DocContent* pDocContent);	
	void serialize(SF::Archive &archive);
private:
	DocContent* m_pDocContent;
};
