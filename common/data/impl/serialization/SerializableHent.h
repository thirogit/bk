#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Hent.h"

class SerializableHent
{
public:
	SerializableHent(Hent* pHent);
	void serialize(SF::Archive &archive);
protected:

	uint32_t&    Id();
	std::wstring& Name();
	std::wstring& City();
	std::wstring& ZipCode();
	std::wstring& Street();
	std::wstring& POBox();
	
	std::wstring& FiscalNo();
	std::wstring& REGON();
	std::wstring& Bank();
	AccountNo&   AccountNumber();
	std::wstring& WetIdNo();
	std::wstring& WetLicNo();
	std::wstring& PhoneNo();		
	std::wstring& Alias();		
	HentNo&      HentNumber();	
	std::wstring& Extras();
	std::wstring& PlateNo();
	HentType&    TypeOfHent();
	bool &       GetSyncIt();
	std::wstring& PersonalNo();
	std::wstring& PersonalIdNo();
	DateTime&    IssueDate();
	std::wstring& IssuePost();
	std::wstring& BankName();
	std::wstring& CellPhoneNo();
	std::wstring& EmailAddress();
	//Latitude&    LatitudePosition();
	//Longitude&   LongitudePosition();
	
	

private:
	Hent* m_pHent;
};
