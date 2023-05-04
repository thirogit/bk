#include "stdafx.h"
#include "CompanyView.h"

CompanyView::CompanyView() 
{
}

CompanyView::CompanyView(const CompanyView& src) 
{
	CopyFrom(src);
}

void CompanyView::CopyFrom(const IFarm* src)
{
	m_name = src->GetName();
	m_street = src->GetStreet();
	m_pobox = src->GetPOBox();
	m_city = src->GetCity();
	m_zip = src->GetZipCode();
	m_phone = src->GetPhoneNo();
	m_nip = src->GetNIP();
	m_farmno = src->GetFarmNo();
	m_wetno = src->GetWetIdNo();
	m_regon = src->GetREGON();	
	m_wetlicenceno = src->GetWetLicNo();
	m_cellphoneno = L"";
	m_emailaddress = L"";
	m_latitude.MakeNull();
	m_longitude.MakeNull();
}

void CompanyView::CopyFrom(const CompanyView& src)
{
	m_name = src.m_name;
	m_street = src.m_street;
	m_pobox = src.m_pobox;
	m_city = src.m_city;
	m_zip = src.m_zip;
	m_phone = src.m_phone;
	m_nip = src.m_nip;
	m_farmno = src.m_farmno;
	m_wetno = src.m_wetno;
	m_regon = src.m_regon;
	m_wetlicenceno = src.m_wetlicenceno;
	m_cellphoneno = src.m_cellphoneno;
	m_emailaddress = src.m_emailaddress;
	m_latitude = src.m_latitude;
	m_longitude = src.m_longitude;

}

const std::wstring& CompanyView::GetName() const
{
	return m_name;
}

void CompanyView:: SetName(const std::wstring& sName)
{
	m_name = sName;
}


const std::wstring&  CompanyView::GetStreet() const
{
	return m_street;
}

void CompanyView:: SetStreet(const std::wstring& sStreet)
{
	m_street = sStreet;
}

const std::wstring&  CompanyView::GetPOBox() const
{
	return m_pobox;
}

void CompanyView::SetPOBox(const std::wstring& sPOBox)
{
	m_pobox = sPOBox;
}


const std::wstring&  CompanyView::GetCity() const
{
	return m_city;
}

void CompanyView::SetCity(const std::wstring& sCity)
{
	m_city = sCity;
}

const std::wstring&  CompanyView::GetZip() const
{
	return m_zip;
}

void CompanyView::SetZip(const std::wstring& sZip)
{
	m_zip = sZip;
}


const std::wstring&  CompanyView::GetPhoneNo() const
{
	return m_phone;
}

void CompanyView::SetPhoneNo(const std::wstring& sPhone)
{
	m_phone = sPhone;
}


const std::wstring&  CompanyView::GetFiscalNo() const
{
	return m_nip;
}

void CompanyView::SetFiscalNo(const std::wstring& sFiscalNo)
{
	m_nip = sFiscalNo;
}

const FarmNo&  CompanyView::GetFarmNo() const
{
	return m_farmno;
}

void CompanyView:: SetFarmNo(const FarmNo& hentNo)
{
	m_farmno = hentNo;
}

const std::wstring&  CompanyView::GetWetNo() const
{
	return m_wetno;
}

void CompanyView:: SetWetNo(const std::wstring& sWetNo)
{
	m_wetno = sWetNo;
}


const std::wstring&  CompanyView::GetREGON() const
{
	return m_regon;
}

void CompanyView:: SetREGON(const std::wstring& sREGON)
{
	m_regon = sREGON;
}

const std::wstring&  CompanyView::GetWetLicenceNo() const
{
	return m_wetlicenceno;
}

void CompanyView::SetWetLicenceNo(const std::wstring& sWetLicenceNo)
{
	m_wetlicenceno = sWetLicenceNo;
}

const std::wstring&  CompanyView::GetCellPhoneNo() const
{
	return m_cellphoneno;
}

void CompanyView::SetCellPhoneNo(const std::wstring& sCellPhoneNo)
{
	m_cellphoneno = sCellPhoneNo;
}

const std::wstring&  CompanyView::GetEmailAddress() const
{
	return m_emailaddress;
}

void CompanyView::SetEmailAddress(const std::wstring& sEmailAddress)
{
	m_emailaddress = sEmailAddress;
}

const Latitude& CompanyView::GetLatitude() const
{
	return m_latitude;
}

void CompanyView::SetLatitude(const Latitude& lati)
{
	m_latitude = lati;
}


const Longitude& CompanyView::GetLongitude() const
{
	return m_longitude;
}

void CompanyView::SetLongitude(const Longitude& longi)
{
	m_longitude = longi;
}



