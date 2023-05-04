#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Herd.h"

class SerializableHerd
{
public:
	SerializableHerd(Herd* pHerd);	
	std::wstring& HerdName();
	std::wstring& City();
	std::wstring& ZipCode();
	std::wstring& Street();
	std::wstring& POBox();	
	int& HerdIndex();
	uint32_t& Id();

	void serialize(SF::Archive &archive);
private:
	Herd* m_pHerd;
};
