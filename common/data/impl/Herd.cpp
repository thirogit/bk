#include "stdafx.h"
#include "Herd.h"

Herd::Herd(uint32_t id) : m_Id(id),m_HerdIndex(0)
{
}

Herd::Herd(const IHerd* pSrc) : m_Id(pSrc->GetId())
{
	CopyFrom(pSrc);
}

const std::wstring& Herd::GetHerdName() const
{
	return m_HerdName;
}

const std::wstring& Herd::GetCity() const
{
	return m_City;
}

const std::wstring& Herd::GetZipCode() const
{
	return m_ZipCode;
}

const std::wstring& Herd::GetStreet() const
{
	return m_Street;
}

const std::wstring& Herd::GetPOBox() const
{
	return m_POBox;
}

int Herd::GetHerdIndex() const
{
	return m_HerdIndex;
}

uint32_t Herd::GetId() const
{
	return m_Id;
}


void Herd::SetHerdName(const std::wstring& sHerdName)
{
	m_HerdName = sHerdName;
}

void Herd::SetCity(const std::wstring& sCity)
{
	m_City = sCity;
}

void Herd::SetZipCode(const std::wstring& sZipCode)
{
	m_ZipCode = sZipCode;
}

void Herd::SetStreet(const std::wstring& sStreet)
{
	m_Street = sStreet;
}

void Herd::SetPOBox(const std::wstring& sPOBox)
{
	m_POBox = sPOBox;
}

void Herd::SetHerdIndex(int herdIndex)
{
	m_HerdIndex = herdIndex;
}
	
void Herd::CopyFrom(const IHerd* pSrc)
{
	SetHerdName(pSrc->GetHerdName());
	SetCity(pSrc->GetCity());
	SetZipCode(pSrc->GetZipCode());
	SetStreet(pSrc->GetStreet());
	SetPOBox(pSrc->GetPOBox());
	SetHerdIndex(pSrc->GetHerdIndex());	
}