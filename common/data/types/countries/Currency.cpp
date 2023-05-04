#include "stdafx.h"
#include "Country.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
InvalidCountry::InvalidCountry() : Country(0)
{
}
//---------------------------------------------------------------------------------------

Country::Country(unsigned int number) : cowNoLength(0),farmNoLength(0)
{
	isoNumber = number;
	inEU = false;
}

Country::Country(const Country& copy)
{
	operator=(copy);
}

Country& Country::operator=(const Country& right)
{
	code2a = right.code2a;
	code3a = right.code3a;
	isoNumber = right.isoNumber;
	inEU = right.inEU;
	currencyCode = right.currencyCode;
	cowNoLength = right.cowNoLength;
	farmNoLength = right.farmNoLength;
	herdNoLength = right.herdNoLength;
	return *this;
}


unsigned int Country::GetCountryISONumber() const
{
	return isoNumber;
}

bool Country::IsCountryInEU() const
{
	return inEU;
}

std::string Country::GetCountryCode2A() const
{
	return code2a;
}

std::string Country::GetCountryCode3A() const
{
	return code3a;
}

std::string Country::GetCurrencyCode() const
{
	return currencyCode;
}
		
void Country::SetCountryInEU(bool bInEU)
{
	inEU = bInEU;
}

void Country::SetCountryCode2A(const std::string& sCountryCode2a)
{
	code2a = sCountryCode2a;
}

void Country::SetCountryCode3A(const std::string& sCountryCode3a)
{
	code3a = sCountryCode3a;
}

void Country::SetCurrencyCode(const std::string& sCurrencyCode)
{
	currencyCode = sCurrencyCode;
}

void Country::SetCowNoLength(int length)
{
	cowNoLength = length;
}

int Country::GetCowNoLength() const
{
	return cowNoLength;
}

void Country::SetFarmNoLength(int length)
{
	farmNoLength = length;
}

void Country::SetHerdNoLength(int length)
{
	herdNoLength = length;
}

int Country::GetHerdNoLength() const
{
	return herdNoLength;
}

int Country::GetFarmNoLength() const
{
	return farmNoLength;
}
