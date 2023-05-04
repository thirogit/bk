#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../../types/FarmNo.h"
#include "../../types/AccountNo.h"
#include "../Farm.h"

class SerializableFarm 
{
public:
	SerializableFarm(Farm* pFarm);

	std::wstring& CompanyName();
	std::wstring& Alias();
	std::wstring& City();
	std::wstring& ZipCode();
	std::wstring& Street();
	std::wstring& POBox();
	FarmNo& FarmNumber();	
	std::wstring& NIP();
	std::wstring& REGON();	
	std::wstring& WetIdNo();
	std::wstring& WetLicNo();
	std::wstring& PhoneNo();	

	void serialize(SF::Archive &archive);

private:
	Farm* m_pFarm;
};
