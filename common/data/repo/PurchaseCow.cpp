#include "stdafx.h"
#include "PurchaseCow.h"

PurchaseCow::PurchaseCow(const std::wstring& id) : m_id(id)
{
}


void PurchaseCow::CopyFrom(const IPurchaseCow* src)
{
	m_CowNo = src->GetCowNo();
	m_Sex = src->GetSex();
	m_StockCode = src->GetStockCd();
	m_ClassCode = src->GetClassCd();
	m_Weight = src->GetWeight();
	m_Price = src->GetPrice();
	m_Latitude = src->GetLatitude();
	m_Longitude = src->GetLongitude();
	m_PassportNo = src->GetPassportNo();
	m_FirstOwnerNo = src->GetFirstOwner();
	m_PassportIssueDt = src->GetPassportIssueDt();
	m_HealthCertNo = src->GetHealthCertNo();
	m_MotherNo = src->GetMotherNo();
	m_BirthPlace = src->GetBirthPlace();
	m_BirthDt = src->GetBirthDt();
}

void PurchaseCow::CopyFrom(const PurchaseCow& src)
{
	m_CowNo = src.m_CowNo;
	m_Sex = src.m_Sex;
	m_StockCode = src.m_StockCode;
	m_Weight = src.m_Weight;
	m_Price = src.m_Price;
	m_Latitude = src.m_Latitude;
	m_Longitude = src.m_Longitude;
	m_PassportNo = src.m_PassportNo;
	m_FirstOwnerNo = src.m_FirstOwnerNo;
	m_PassportIssueDt = src.m_PassportIssueDt;
	m_HealthCertNo = src.m_HealthCertNo;
	m_MotherNo = src.m_MotherNo;
	m_BirthPlace = src.m_BirthPlace;
	m_BirthDt = src.m_BirthDt;
}

const std::wstring& PurchaseCow::GetId() const
{
	return m_id;
}

const CowNo& PurchaseCow::GetCowNo() const
{
	return m_CowNo; 
}

const CowSex& PurchaseCow::GetSex() const
{
	return m_Sex;
}
	
const std::wstring& PurchaseCow::GetStockCd() const
{
	return m_StockCode;
}

const std::wstring& PurchaseCow::GetClassCd() const
{
	return m_ClassCode;
}

const Decimal& PurchaseCow::GetWeight() const
{
	return m_Weight;
}
	
const Decimal& PurchaseCow::GetPrice() const
{
	return m_Price;
}
	
const Latitude& PurchaseCow::GetLatitude() const
{
	return m_Latitude;
}

const Longitude& PurchaseCow::GetLongitude() const
{
	return m_Longitude;
}

const std::wstring& PurchaseCow::GetPassportNo() const
{
	return m_PassportNo;
}
	
const HentNo& PurchaseCow::GetFirstOwner() const
{
	return m_FirstOwnerNo;
}

const DateTime& PurchaseCow::GetPassportIssueDt() const
{
	return m_PassportIssueDt;
}

const std::wstring& PurchaseCow::GetHealthCertNo() const
{
	return m_HealthCertNo;
}

const CowNo& PurchaseCow::GetMotherNo() const
{
	return m_MotherNo;
}

const std::wstring& PurchaseCow::GetBirthPlace() const
{
	return m_BirthPlace;
}

const DateTime& PurchaseCow::GetBirthDt() const
{
	return m_BirthDt;
}

void PurchaseCow::SetCowNo(const CowNo& cowNo)
{
	m_CowNo = cowNo;
}

void PurchaseCow::SetSex(const CowSex& sex)
{
	m_Sex = sex;
}

void PurchaseCow::SetClassCd(const std::wstring& sClassCd)
{
	m_ClassCode = sClassCd;
}
	
void PurchaseCow::SetStockCd(const std::wstring& sStockCode)
{
	m_StockCode = sStockCode;
}

void PurchaseCow::SetWeight(const Decimal& weight)
{
	m_Weight = weight;
}
	
void PurchaseCow::SetPrice(const Decimal& price)
{
	m_Price = price;
}
	
void PurchaseCow::SetLatitude(const Latitude& latitude)
{
	m_Latitude = latitude;
}

void PurchaseCow::SetLongitude(const Longitude& longitude)
{
	m_Longitude = longitude;
}

void PurchaseCow::SetPassportNo(const std::wstring& sPassportNo)
{
	m_PassportNo = sPassportNo;
}
	
void PurchaseCow::SetFirstOwner(const HentNo& firstOwnerNo)
{
	m_FirstOwnerNo = firstOwnerNo;
}

void PurchaseCow::SetPassportIssueDt(const DateTime& passportIssueDt)
{
	m_PassportIssueDt = passportIssueDt;
}

void PurchaseCow::SetHealthCertNo(const std::wstring& sHealthCertNo)
{
	m_HealthCertNo = sHealthCertNo;
}

void PurchaseCow::SetMotherNo(const CowNo& motherNo)
{
	m_MotherNo = motherNo;
}

void PurchaseCow::SetBirthPlace(const std::wstring& sBirthPlace)
{
	m_BirthPlace = sBirthPlace;
}

void PurchaseCow::SetBirthDt(const DateTime& birthDt)
{
	m_BirthDt = birthDt;
}