#include "stdafx.h"
#include "Farm.h"

Farm::Farm() : m_Id(NULL_ID)
{
}

Farm::Farm(uint32_t Id) : m_Id(Id)
{
}

Farm::Farm(const IFarm* src) : m_Id(src->GetId())
{
	CopyFrom(src);
}

uint32_t Farm::GetId() const
{
	return m_Id;
}

const std::wstring& Farm::GetName() const
{
	return m_Name;
}

const std::wstring& Farm::GetAlias() const
{
	return m_Alias;
}

const std::wstring& Farm::GetCity() const
{
	return m_City;
}

const std::wstring& Farm::GetZipCode() const
{
	return m_ZipCode;
}

const std::wstring& Farm::GetStreet() const
{
	return m_Street;
}

const std::wstring& Farm::GetPOBox() const
{
	return m_POBox;
}

const FarmNo& Farm::GetFarmNo() const
{
	return m_FarmNo;
}
	
const std::wstring& Farm::GetNIP() const
{
	return m_NIP;
}

const std::wstring& Farm::GetREGON() const
{
	return m_REGON;
}


const std::wstring& Farm::GetWetIdNo() const
{
	return m_WetIdNo;
}

const std::wstring& Farm::GetWetLicNo() const
{
	return m_WetLicNo;
}

const std::wstring& Farm::GetPhoneNo() const
{
	return m_PhoneNo;
}


void Farm::SetName(const std::wstring& sCompanyName)
{
	m_Name = sCompanyName;
}

void Farm::SetAlias(const std::wstring& alias)
{
	m_Alias = alias;
}

void Farm::SetCity(const std::wstring& sCity)
{
	m_City = sCity;
}

void Farm::SetZipCode(const std::wstring& sZipCode)
{
	m_ZipCode = sZipCode;
}

void Farm::SetStreet(const std::wstring& sStreet)
{
	m_Street = sStreet;
}

void Farm::SetPOBox(const std::wstring& sPOBox)
{
	m_POBox = sPOBox;
}

void Farm::SetFarmNo(const FarmNo& farmNo)
{
	m_FarmNo = farmNo;
}
	
void Farm::SetNIP(const std::wstring& sNIP)
{
	m_NIP = sNIP;
}

void Farm::SetREGON(const std::wstring& sREGON)
{
	m_REGON = sREGON;
}

void Farm::SetWetIdNo(const std::wstring& sWetIdNo)
{
	m_WetIdNo = sWetIdNo;
}

void Farm::SetWetLicNo(const std::wstring& sWetLicNo)
{
	m_WetLicNo = sWetLicNo;
}

void Farm::SetPhoneNo(const std::wstring& sPhoneNo)
{
	m_PhoneNo = sPhoneNo;
}

void Farm::CopyFrom(const IFarm* pSrc)
{
	SetName(pSrc->GetName());
	SetAlias(pSrc->GetAlias());
	SetCity(pSrc->GetCity());
	SetZipCode(pSrc->GetZipCode());
	SetStreet(pSrc->GetStreet());
	SetPOBox(pSrc->GetPOBox());
	SetFarmNo(pSrc->GetFarmNo());	
	SetNIP(pSrc->GetNIP());
	SetREGON(pSrc->GetREGON());	
	SetWetIdNo(pSrc->GetWetIdNo());
	SetWetLicNo(pSrc->GetWetLicNo());
	SetPhoneNo(pSrc->GetPhoneNo());	
}