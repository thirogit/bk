#include "stdafx.h"
#include "HerdView.h"

HerdView::HerdView(uint32_t hentId) :  m_Id(hentId) 
{
}

HerdView::HerdView(const HerdView& src) : m_Id(src.GetHerdId())
{
	CopyFrom(src);
}

HerdView& HerdView::operator=(const HerdView& src)
{
	CopyFrom(src);
	return *this;
}

void HerdView::CopyFrom(const HerdView& src)
{
	m_name = src.m_name;
	m_street = src.m_street;
	m_pobox = src.m_pobox;
	m_city = src.m_city;
	m_zip = src.m_zip;
}

uint32_t HerdView::GetHerdId() const
{
	return m_Id;
}

const std::wstring& HerdView::GetName() const
{
	return m_name;
}

void HerdView:: SetName(const std::wstring& sName)
{
	m_name = sName;
}

const std::wstring&  HerdView::GetStreet() const
{
	return m_street;
}

void HerdView::SetStreet(const std::wstring& sStreet)
{
	m_street = sStreet;
}

const std::wstring&  HerdView::GetPOBox() const
{
	return m_pobox;
}

void HerdView::SetPOBox(const std::wstring& sPOBox)
{
	m_pobox = sPOBox;
}


const std::wstring&  HerdView::GetCity() const
{
	return m_city;
}

void HerdView::SetCity(const std::wstring& sCity)
{
	m_city = sCity;
}

const std::wstring&  HerdView::GetZip() const
{
	return m_zip;
}

void HerdView::SetZip(const std::wstring& sZip)
{
	m_zip = sZip;
}



