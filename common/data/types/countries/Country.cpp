#include "stdafx.h"
#include "Country.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------

Country::Country() 
{
}

void Country::MakeNull()
{
	m_Value.reset();
}

bool Country::IsNull() const
{
	return !m_Value.is_initialized();
}


Country::Country(const Country& copy) : m_Value(copy.m_Value)
{
	
}

Country& Country::operator=(const Country& right)
{	
	m_Value = right.m_Value;
	return *this;
}


unsigned int Country::GetCountryISONumber() const
{
	if (IsNull())
		return 0;

	return m_Value.get().isoNumber;
}

bool Country::IsCountryInEU() const
{
	if (IsNull())
		return false;

	return m_Value.get().inEU;
}

std::wstring Country::GetCountryCode2A() const
{
	if (IsNull())
		return L"";

	return m_Value.get().code2a;
}

std::wstring Country::GetCountryCode3A() const
{
	if (IsNull())
		return L"";

	return m_Value.get().code3a;
}

std::wstring Country::GetCurrencyCode() const
{
	if (IsNull())
		return L"";

	return m_Value.get().currencyCode;
}
		

int Country::GetCowNoLength() const
{
	if (IsNull())
		return 0;

	return m_Value.get().cowNoLength;
}

int Country::GetHerdNoLength() const
{
	if (IsNull())
		return 0;

	return m_Value.get().herdNoLength;
}

int Country::GetFarmNoLength() const
{
	if (IsNull())
		return 0;

	return m_Value.get().farmNoLength;
}
