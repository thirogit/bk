#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Agent.h"

class SerializableAgent
{
public:
	SerializableAgent(Agent* pAgent);

	uint32_t&		Id();
	std::wstring& 	Name();	
	std::wstring& 	Code();	
	std::wstring& 	Plate();	
	
	void serialize(SF::Archive &archive);

private:
	Agent* m_pAgent;
};
