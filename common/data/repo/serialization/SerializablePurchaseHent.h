#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../PurchaseHent.h"

class SerializablePurchaseHent
{
public:
	SerializablePurchaseHent(PurchaseHent* pHent);
	void serialize(SF::Archive &archive);
protected:
	std::wstring& Id();
	std::wstring& Name();
	std::wstring& Alias();
	std::wstring& City();
	std::wstring& ZipCode();
	std::wstring& Street();
	std::wstring& POBox();
	
	std::wstring& FiscalNo();
	std::wstring& StatsNo();
	std::wstring& Bank();
	AccountNo&   AccountNumber();
	std::wstring& WetIdNo();
	std::wstring& WetLicNo();
	std::wstring& PhoneNo();		
		
	HentNo&      HentNumber();	
	std::wstring& Extras();
	std::wstring& PlateNo();
	HentType&    TypeOfHent();
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
	PurchaseHent* m_pHent;
};
