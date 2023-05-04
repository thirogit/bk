#include "stdafx.h"
#include "HentNo.h"
#include "../../string/TextUtils.h"
#include "../types/countries/Countries.h"


HentNo HentNoFactory::Create(const Country& country,const std::wstring& number)
{
	if(!TextUtils::IsNumber(number))
		BOOST_THROW_EXCEPTION( HentNoException(TextUtils::Format(L"number part is not all digits: %s", number)));

	if(country.GetFarmNoLength()+country.GetHerdNoLength() != number.size()+2)
		BOOST_THROW_EXCEPTION( HentNoException(TextUtils::Format(L"number part of hent# '%s%s' should be %d characters in length (where first 2 for country code)", country.GetCountryCode2A(), number ,country.GetFarmNoLength())) );

	std::wstring sFarmNoNumPart = TextUtils::Left(number,country.GetFarmNoLength()-2);
	std::wstring sHerdNo = TextUtils::Right(number,country.GetHerdNoLength());
	int herdIndex = widetoi(sHerdNo.c_str());

	try
	{
		FarmNo farmNo = FarmNoFactory::Create(country, sFarmNoNumPart);
		HerdNo herdNo = HerdNoFactory::Create(herdIndex);
		return HentNo(farmNo, herdNo);
	}
	catch (HerdNoException& e)
	{
		BOOST_THROW_EXCEPTION(HentNoException(e.message()));
	}
	catch (FarmNoException& e)
	{
		BOOST_THROW_EXCEPTION(HentNoException(e.message()));
	}

	return HentNo();	
}

HentNo HentNoFactory::Create(const FarmNo& farmNo,const HerdNo& herdNo)
{
	if(farmNo.IsNull())
		BOOST_THROW_EXCEPTION( HentNoException(L"farm no cannot be null") );

	return HentNo(farmNo,herdNo);
}

HentNo HentNoFactory::FromString(const std::wstring& sFarmNo)
{
	if (sFarmNo.empty())
	{
		return HentNo();
	}

	if(sFarmNo.size() < 3)
		BOOST_THROW_EXCEPTION( HentNoException(TextUtils::Format(L"to short to be a farm#: %s", sFarmNo)) );

	std::wstring countryCd = sFarmNo.substr(0,2);
	std::wstring numberPart = sFarmNo.substr(2);
	
	try
	{
		Country country = Countries::GetCountryByCode2a(countryCd);
		return Create(country, numberPart);
	}
	catch (CountriesException& e)
	{
		BOOST_THROW_EXCEPTION( HentNoException((TextUtils::Format(L"unknown country code: %s in hent number %s", countryCd, sFarmNo))) );
	}	
}


bool HentNo::InternalHentNo::operator==(const HentNo::InternalHentNo& right) const
{
	return m_FarmNo == right.m_FarmNo && m_HerdNo == right.m_HerdNo;
}


HentNo::HentNo()
{
#if _DEBUG
	m_stringValue = ToString();
#endif
}

HentNo::HentNo(const FarmNo& farmNo,const HerdNo& herdNo)
{
	InternalHentNo hentNo;
	hentNo.m_FarmNo = farmNo;
	hentNo.m_HerdNo = herdNo;
	m_Value = hentNo;
#if _DEBUG
	m_stringValue = ToString();
#endif
}
	

HentNo::HentNo(const HentNo& hentNo) : m_Value(hentNo.m_Value)
{
#if _DEBUG
	m_stringValue = ToString();
#endif
}

std::wstring HentNo::ToString() const
{
	if(IsNull())
		return L"";

	auto intHentNo = m_Value.get();

	Country country = intHentNo.m_FarmNo.GetCountry();
	return country.GetCountryCode2A() + intHentNo.m_FarmNo.GetNumber() + intHentNo.m_HerdNo.ToString();
}

HentNo& HentNo::operator=(const HentNo& right)
{
	CopyFrom(right);
	return *this;
}

std::wstring  HentNo::GetNumber() const
{
	if(IsNull())
		return L"";

	return m_Value.get().m_FarmNo.GetNumber() + m_Value.get().m_HerdNo.ToString();
}

Country HentNo::GetCountry() const
{
	return m_Value.get().m_FarmNo.GetCountry();
}

void HentNo::CopyFrom(const HentNo& src)
{
	m_Value = src.m_Value;
#if _DEBUG
	m_stringValue = ToString();
#endif
}

FarmNo HentNo::GetFarmNo() const
{
	if(IsNull())
		return FarmNo();

	return m_Value.get().m_FarmNo;
}

HerdNo HentNo::GetHerdNo() const
{
	if(IsNull())
		return HerdNo();

	return m_Value.get().m_HerdNo;
}

void HentNo::MakeNull()
{
	m_Value.reset();
#if _DEBUG
	m_stringValue = ToString();
#endif
}

bool HentNo::IsNull() const
{
	return !m_Value.is_initialized();
}

bool operator>(const HentNo& left, const HentNo& right)
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
			
			if(leftValue.m_FarmNo == rightValue.m_FarmNo)
			{
				return leftValue.m_HerdNo > rightValue.m_HerdNo;
			}

			return leftValue.m_FarmNo > rightValue.m_FarmNo;
		}
	}
	return false; 
}

bool operator<(const HentNo& left, const HentNo& right)
{
	return right > left;
}

bool operator==(const HentNo& left, const HentNo& right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_Value.get() == right.m_Value.get());
}

bool operator!=(const HentNo& left, const HentNo& right)
{
	return !(left == right);
}
