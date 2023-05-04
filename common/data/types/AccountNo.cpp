#include "stdafx.h"
#include "AccountNo.h"
#include "../../string/TextUtils.h"
#include "../types/countries/Countries.h"

AccountNo AccountNoFactory::Create(const Country& country,const std::wstring& number)
{
	if(!TextUtils::IsNumber(number))
		BOOST_THROW_EXCEPTION(AccountNoException(TextUtils::Format(L"number part in account no %s%s is not all digits",country.GetCountryCode2A(),number)));

	if(26 != number.size())
		BOOST_THROW_EXCEPTION(AccountNoException(TextUtils::Format(L"bad account no %s%s, number part should be %d digits in length", country.GetCountryCode2A(), number, 26)));

	return AccountNo(country,number);
}

AccountNo AccountNoFactory::FromString(const std::wstring& sAccountNo)
{
	if(sAccountNo.empty())
		return AccountNo();

	if(sAccountNo.size() < 5)
		BOOST_THROW_EXCEPTION(AccountNoException(L"to short to be a account#"));

	std::wstring countryCd = sAccountNo.substr(0,2);
	std::wstring numberPart = sAccountNo.substr(2);
	 
	try
	{
		Country country = Countries::GetCountryByCode2a(countryCd);
		return Create(country, numberPart);
	}
	catch (CountriesException& e)
	{
		BOOST_THROW_EXCEPTION(AccountNoException(TextUtils::Format(L"unknown country code: %s in account number %s", countryCd, sAccountNo)));
	}
	
}

//-------------------------------------------------------------------------------------------------

AccountNo::InternalAccountNo::InternalAccountNo() 
{
}

//-------------------------------------------------------------------------------------------------

AccountNo::AccountNo() 
{
}

AccountNo::AccountNo(const Country& country,const std::wstring& number) 
{
	InternalAccountNo accountNo;
	accountNo.m_Country = country;
	accountNo.m_Number = number;
	m_Value = accountNo;
#if _DEBUG
	m_stringValue = ToString();
#endif
}

AccountNo::AccountNo(const AccountNo& accountNo) : m_Value(accountNo.m_Value)
{
#if _DEBUG
	m_stringValue = ToString();
#endif
}

void AccountNo::MakeNull()
{
	m_Value.reset();
#if _DEBUG
	m_stringValue = ToString();
#endif
}

bool AccountNo::IsNull() const
{
	return !m_Value.is_initialized();
}

std::wstring AccountNo::ToString() const
{
	if(IsNull())
		return L"";

	return m_Value->m_Country.GetCountryCode2A() + m_Value->m_Number;
}

AccountNo& AccountNo::operator=(const AccountNo& right)
{
	m_Value = right.m_Value;	
#if _DEBUG
	m_stringValue = ToString();
#endif
	return *this;
}

Country AccountNo::GetCountry() const
{
	if(IsNull())
		return Country();

	return m_Value->m_Country;
}

std::wstring AccountNo::GetNumber() const
{
	if(IsNull())
		return L"";

	return m_Value->m_Number;
}

bool operator>=(const AccountNo& left, const AccountNo& right)
{
	if(left == right)
		return true;

	return left > right;
}

bool operator<=(const AccountNo& left, const AccountNo& right)
{
	if(left == right)
		return true;

	return left < right;
}

bool operator>(const AccountNo& left, const AccountNo& right)
{
	int countryCmpResult = left.GetCountry().GetCountryCode2A().compare(right.GetCountry().GetCountryCode2A());

	if(countryCmpResult == 0)
		return left.GetNumber() > right.GetNumber();

	return countryCmpResult > 0;

}

bool operator<(const AccountNo& left, const AccountNo& right)
{
	return right > left;
}

bool operator==(const AccountNo& left, const AccountNo& right)
{
	int countryCmpResult = left.GetCountry().GetCountryCode2A().compare(right.GetCountry().GetCountryCode2A());

	if(countryCmpResult == 0)
		return left.GetNumber() == right.GetNumber();

	return false;
}

bool operator!=(const AccountNo& left, const AccountNo& right)
{
	return !(left == right);
}
