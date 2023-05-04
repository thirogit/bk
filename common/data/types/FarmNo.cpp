#include "stdafx.h"
#include "FarmNo.h"
#include "../../string/TextUtils.h"
#include "../types/countries/Countries.h"


FarmNo FarmNoFactory::Create(const Country& country,const std::wstring& number)
{
	if(!TextUtils::IsNumber(number))
		BOOST_THROW_EXCEPTION(FarmNoException(L"number part is not all digits"));

	if(country.GetFarmNoLength() != number.size()+2)
		BOOST_THROW_EXCEPTION(FarmNoException(TextUtils::Format(L"number part should be %d digits in length", country.GetFarmNoLength())));

	return FarmNo(country,number);
}

FarmNo FarmNoFactory::FromString(const std::wstring& sFarmNo)
{
	if(sFarmNo.size() < 3)
		BOOST_THROW_EXCEPTION(FarmNoException(L"to short to be a farm#"));

	std::wstring countryCd = sFarmNo.substr(0,2);
	std::wstring numberPart = sFarmNo.substr(2);
	
	try
	{
		Country country = Countries::GetCountryByCode2a(countryCd);

		return Create(country, numberPart);
	}
	catch (CountriesException& e)
	{
		BOOST_THROW_EXCEPTION(FarmNoException(TextUtils::Format(L"unknown country code: %s in farm number %s", countryCd, sFarmNo)));
	}
}


FarmNo::InternalFarmNo::InternalFarmNo() 
{
}

bool FarmNo::InternalFarmNo::operator==(const FarmNo::InternalFarmNo& right) const
{
	if(m_Country.GetCountryCode2A() == right.m_Country.GetCountryCode2A())
	{
		return m_Number == right.m_Number;
	}
	return false;		
}

FarmNo::FarmNo() 
{
}

FarmNo::FarmNo(Country country,const std::wstring& number) 
{
	InternalFarmNo farmNo;
	farmNo.m_Country = country;
	farmNo.m_Number = number;
	m_Value = farmNo;
}

FarmNo::FarmNo(const FarmNo& farmNo) 
{	
	CopyFrom(farmNo);
}

std::wstring FarmNo::ToString() const
{
	if(IsNull())
		return L"";
	
	auto value = m_Value.get();
	return value.m_Country.GetCountryCode2A() + value.m_Number;
}

FarmNo& FarmNo::operator=(const FarmNo& right)
{
	CopyFrom(right);
	return *this;
}

Country FarmNo::GetCountry() const
{
	if(IsNull())
		return Country();

	return m_Value.get().m_Country;
}

std::wstring FarmNo::GetNumber() const
{
	if(IsNull())
		return L"";

	return m_Value.get().m_Number;
}

void FarmNo::CopyFrom(const FarmNo& src)
{
	m_Value = src.m_Value;	
}

void FarmNo::MakeNull()
{
	m_Value.reset();
}

bool FarmNo::IsNull() const
{
	return !m_Value.is_initialized();
}

bool operator==(const FarmNo& left, const FarmNo& right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_Value.get() == right.m_Value.get());
}


bool operator>(const FarmNo& left, const FarmNo& right)
{
	if(!left.IsNull()) 
	{
		if(right.IsNull())
		{
			return true; 
		}
		else 
		{
			auto leftValue = left.m_Value.get();
			auto rightValue = right.m_Value.get();

			int countryCmpResult = leftValue.m_Country.GetCountryCode2A().compare(rightValue.m_Country.GetCountryCode2A());

			if(countryCmpResult == 0)
			{
				return leftValue.m_Number > rightValue.m_Number;
			}

			return countryCmpResult > 0;
		}
	}
	return false; 

}

bool operator<(const FarmNo& left, const FarmNo& right)
{
	return right > left;
}