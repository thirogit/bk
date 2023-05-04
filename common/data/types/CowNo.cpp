#include "stdafx.h"
#include "CowNo.h"
#include "../../string/TextUtils.h"
#include "../types/countries/Countries.h"


CowNo CowNoFactory::Create(const Country& country,const std::wstring& number)
{
	if(!TextUtils::IsNumber(number))
		BOOST_THROW_EXCEPTION(CowNoException(TextUtils::Format(L"number part in cow no %s%s is not all digits", country.GetCountryCode2A(), number)) );

	int correctNumberPartLength = country.GetCowNoLength() - 2;

	if (correctNumberPartLength != number.size())
		BOOST_THROW_EXCEPTION( CowNoException(TextUtils::Format(L"bad cowno no %s%s, number part in should be %d digits in length ", country.GetCountryCode2A(), number, correctNumberPartLength)) );

	return CowNo(country,number);
}

CowNo CowNoFactory::FromString(const std::wstring& sCowNo)
{
	if(sCowNo.empty())
		return CowNo();

	if(sCowNo.size() < 3)
		BOOST_THROW_EXCEPTION( CowNoException(L"to short to be a cow#"));

	std::wstring countryCd = sCowNo.substr(0,2);
	std::wstring numberPart = sCowNo.substr(2);
	
	try
	{
		Country country = Countries::GetCountryByCode2a(countryCd);
		return Create(country, numberPart);
	}
	catch (CountriesException& e)
	{
		BOOST_THROW_EXCEPTION(CowNoException(TextUtils::Format(L"unknown country code: %s in cow number %s", countryCd, sCowNo)));
	}
	

	
}
//-------------------------------------------------------------------------------------------------

CowNo::InternalCowNo::InternalCowNo()  {}


bool CowNo::InternalCowNo::operator==(const CowNo::InternalCowNo& right) const
{
	if(m_Country.GetCountryISONumber() == right.m_Country.GetCountryISONumber())
		return m_Number == right.m_Number;

	return false;
}
//-------------------------------------------------------------------------------------------------
CowNo::CowNo() 
{
}

CowNo::CowNo(Country country,const std::wstring& number) 
{
	InternalCowNo cowNo;
	cowNo.m_Country = country;
	cowNo.m_Number = number;
	m_Value = cowNo;
}

CowNo::CowNo(const CowNo& cowNo) : m_Value(cowNo.m_Value)
{	
}

std::wstring CowNo::ToString() const
{
	if(IsNull())
		return L"";

	return m_Value->m_Country.GetCountryCode2A() + m_Value->m_Number;
}

CowNo& CowNo::operator=(const CowNo& right)
{
	CopyFrom(right);
	return *this;
}

Country CowNo::GetCountry() const
{
	if(IsNull())
		return Country();

	return m_Value->m_Country;
}

std::wstring CowNo::GetNumber() const
{
	if(IsNull())
		return L"";

	return m_Value->m_Number;
}

void CowNo::CopyFrom(const CowNo& src)
{
	m_Value = src.m_Value;
}

void CowNo::MakeNull()
{
	m_Value.reset();
}

bool CowNo::IsNull() const
{
	return !m_Value.is_initialized();
}

bool operator>(const CowNo& left, const CowNo& right)
{
	int countryCmpResult = left.GetCountry().GetCountryCode2A().compare(right.GetCountry().GetCountryCode2A());

	if(countryCmpResult == 0)
		return left.GetNumber() > right.GetNumber();

	return countryCmpResult > 0;

}

bool operator<(const CowNo& left, const CowNo& right)
{
	return right > left;
}

bool operator!=(const CowNo& left, const CowNo& right)
{
	return !(left == right);
}

bool operator==(const CowNo& left, const CowNo& right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();

	if(l && r) return true;
	if(l || r) return false;

	return left.m_Value.get() == right.m_Value.get();

}