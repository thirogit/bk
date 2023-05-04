#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 
#include "../print/PrintProfile.h"

class SerializablePrintProfile
{
public:
	SerializablePrintProfile(PrintProfile* pProfile);
	void serialize(SF::Archive &archive);

private:
	uint32_t&		Id();
	std::wstring& 	ProfileName();	
	std::wstring& 	DocumentCode();

private:
	PrintProfile* m_pProfile;
};
