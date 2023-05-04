#include "stdafx.h"
#include "SerializableFarm.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableFarm::SerializableFarm(Farm* pFarm) : m_pFarm(pFarm)
{

}

std::wstring& SerializableFarm::CompanyName()
{
	return m_pFarm->m_Name;
}

std::wstring& SerializableFarm::Alias()
{
	return m_pFarm->m_Alias;
}

std::wstring& SerializableFarm::City()
{
	return m_pFarm->m_City;
}

std::wstring& SerializableFarm::ZipCode()
{
	return m_pFarm->m_ZipCode;
}

std::wstring& SerializableFarm::Street()
{
	return m_pFarm->m_Street;
}

std::wstring& SerializableFarm::POBox()
{
	return m_pFarm->m_POBox;
}

FarmNo& SerializableFarm::FarmNumber()
{
	return m_pFarm->m_FarmNo;
}
	
std::wstring& SerializableFarm::NIP()
{
	return m_pFarm->m_NIP;
}

std::wstring& SerializableFarm::REGON()
{
	return m_pFarm->m_REGON;
}

std::wstring& SerializableFarm::WetIdNo()
{
	return m_pFarm->m_WetIdNo;
}

std::wstring& SerializableFarm::WetLicNo()
{
	return m_pFarm->m_WetLicNo;
}

std::wstring& SerializableFarm::PhoneNo()
{
	return m_pFarm->m_PhoneNo;
}
	
void SerializableFarm::serialize(SF::Archive &archive)
{
	archive & CompanyName()
			& Alias()
            & City()
			& ZipCode()
			& Street()
			& POBox()
			& FarmNumber()
			& NIP()
			& REGON()			
			& WetIdNo()
			& WetLicNo()
			& PhoneNo();	
}