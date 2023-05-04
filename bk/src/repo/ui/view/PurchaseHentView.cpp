#include "stdafx.h"
#include "PurchaseHentView.h"

PurchaseHentView::PurchaseHentView(const std::wstring& hentId) : m_id(hentId)
{

}

const std::wstring& PurchaseHentView::GetId() const
{
	return m_id;
}

void PurchaseHentView::CopyFrom(const PurchaseHentView& src)
{
	m_name = src.m_name;
	m_alias = src.m_alias;
	m_street = src.m_street;
	m_pobox = src.m_pobox;
	m_city = src.m_city;
	m_zip = src.m_zip;
	m_phone = src.m_phone;
	m_nip = src.m_nip;
	m_hentno = src.m_hentno;
	m_wetno = src.m_wetno;	
	m_plate = src.m_plate;
	m_henttype = src.m_henttype;	
	m_pesel = src.m_pesel;
	m_regon = src.m_regon;
	m_idno = src.m_idno;
	m_issuedate = src.m_issuedate;
	m_issuepost = src.m_issuepost;
	m_accountno = src.m_accountno;
	m_bankname = src.m_bankname;
	m_wetlicenceno = src.m_wetlicenceno;
	m_cellphoneno = src.m_cellphoneno;
	m_emailaddress = src.m_emailaddress;
	m_latitude = src.m_latitude;
	m_longitude = src.m_longitude;
}

const std::wstring& PurchaseHentView::GetName() const
{
	return m_name;
}

void PurchaseHentView::SetName(const std::wstring& sName)
{
	m_name = sName;
}

const std::wstring&  PurchaseHentView::GetAlias() const
{
	return m_alias;
}

void PurchaseHentView::SetAlias(const std::wstring& sAlias)
{
	m_alias = sAlias;
}

const std::wstring&  PurchaseHentView::GetStreet() const
{
	return m_street;
}

void PurchaseHentView::SetStreet(const std::wstring& sStreet)
{
	m_street = sStreet;
}

const std::wstring&  PurchaseHentView::GetPOBox() const
{
	return m_pobox;
}

void PurchaseHentView::SetPOBox(const std::wstring& sPOBox)
{
	m_pobox = sPOBox;
}


const std::wstring&  PurchaseHentView::GetCity() const
{
	return m_city;
}

void PurchaseHentView::SetCity(const std::wstring& sCity)
{
	m_city = sCity;
}

const std::wstring&  PurchaseHentView::GetZip() const
{
	return m_zip;
}

void PurchaseHentView::SetZip(const std::wstring& sZip)
{
	m_zip = sZip;
}


const std::wstring&  PurchaseHentView::GetPhoneNo() const
{
	return m_phone;
}

void PurchaseHentView::SetPhoneNo(const std::wstring& sPhone)
{
	m_phone = sPhone;
}


const std::wstring&  PurchaseHentView::GetFiscalNo() const
{
	return m_nip;
}

void PurchaseHentView::SetFiscalNo(const std::wstring& sFiscalNo)
{
	m_nip = sFiscalNo;
}

const HentNo&  PurchaseHentView::GetHentNo() const
{
	return m_hentno;
}

void PurchaseHentView::SetHentNo(const HentNo& hentNo)
{
	m_hentno = hentNo;
}

const std::wstring&  PurchaseHentView::GetWetNo() const
{
	return m_wetno;
}

void PurchaseHentView::SetWetNo(const std::wstring& sWetNo)
{
	m_wetno = sWetNo;
}

const std::wstring&  PurchaseHentView::GetPlateNo() const
{
	return m_plate;
}

void PurchaseHentView::SetPlateNo(const std::wstring& sPlate)
{
	m_plate = sPlate;
}

HentType PurchaseHentView::GetHentType() const
{
	return m_henttype;
}

void PurchaseHentView::SetHentType(HentType hentType)
{
	m_henttype = hentType;
}

const std::wstring&  PurchaseHentView::GetPersonalNo() const
{
	return m_pesel;
}

void PurchaseHentView::SetPersonalNo(const std::wstring& sPESEL)
{
	m_pesel = sPESEL;
}

const std::wstring&  PurchaseHentView::GetStatsNo() const
{
	return m_regon;
}

void PurchaseHentView::SetStatsNo(const std::wstring& sREGON)
{
	m_regon = sREGON;
}

const std::wstring&  PurchaseHentView::GetIdNo() const
{
	return m_idno;
}

void PurchaseHentView::SetIdNo(const std::wstring& sIdNo)
{
	m_idno = sIdNo;
}

const DateTime& PurchaseHentView::GetIssueDate() const
{
	return m_issuedate;
}

void PurchaseHentView::SetIssueDate(const DateTime& IdIssueDate)
{
	m_issuedate = IdIssueDate;
}

const std::wstring&  PurchaseHentView::GetIssuePost() const
{
	return m_issuepost;
}

void PurchaseHentView::SetIssuePost(const std::wstring& sIssuePost)
{
	m_issuepost = sIssuePost;
}


const AccountNo&  PurchaseHentView::GetAccountNo() const
{
	return m_accountno;
}

void PurchaseHentView::SetAccountNo(const AccountNo& accountNo)
{
	m_accountno = accountNo;
}

const std::wstring&  PurchaseHentView::GetBankName() const
{
	return m_bankname;
}

void PurchaseHentView::SetBankName(const std::wstring& sBankName)
{
	m_bankname = sBankName;
}

const std::wstring&  PurchaseHentView::GetWetLicenceNo() const
{
	return m_wetlicenceno;
}

void PurchaseHentView::SetWetLicenceNo(const std::wstring& sWetLicenceNo)
{
	m_wetlicenceno = sWetLicenceNo;
}

const std::wstring&  PurchaseHentView::GetCellPhoneNo() const
{
	return m_cellphoneno;
}

void PurchaseHentView::SetCellPhoneNo(const std::wstring& sCellPhoneNo)
{
	m_cellphoneno = sCellPhoneNo;
}

const std::wstring&  PurchaseHentView::GetEmailAddress() const
{
	return m_emailaddress;
}

void PurchaseHentView::SetEmailAddress(const std::wstring& sEmailAddress)
{
	m_emailaddress = sEmailAddress;
}

const Latitude& PurchaseHentView::GetLatitude() const
{
	return m_latitude;
}

void PurchaseHentView::SetLatitude(const Latitude& lati)
{
	m_latitude = lati;
}

const Longitude& PurchaseHentView::GetLongitude() const
{
	return m_longitude;
}

void PurchaseHentView::SetLongitude(const Longitude& longi)
{
	m_longitude = longi;
}

