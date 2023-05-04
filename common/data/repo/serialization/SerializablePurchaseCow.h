#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../PurchaseCow.h"

class SerializablePurchaseCow
{
public:
	SerializablePurchaseCow(PurchaseCow* pCow);

	std::wstring& Id();
	Decimal& Weight();
	std::wstring& StockCd();
	std::wstring&  ClassCd();
	CowNo& CowNumber();	
	CowNo& MotherNo();
	DateTime& BirthDate();
	std::wstring& PassportNo();
	std::wstring& HealthCertNo();
	DateTime& PassportIssueDt();
	std::wstring& BirthPlace();
	HentNo& FirstOwnerNo();
	CowSex&	Sex();	
	Decimal& Price();
	
	
	void serialize(SF::Archive &archive);

private:
	PurchaseCow* m_pCow;
};
