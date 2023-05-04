#include "stdafx.h"
#include "HentView.h"

HentView::HentView(uint32_t hentId) :  m_Id(hentId) ,m_henttype(HentType_None),m_Locked(false)
{
}

HentView::HentView(const HentView& src) : m_Id(src.GetHentId())
{
	CopyFrom(src);
}

HentView& HentView::operator=(const HentView& src)
{
	CopyFrom(src);
	return *this;
}

void HentView::CopyFrom(const HentView& src)
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
	m_extras = src.m_extras;	
	m_plate = src.m_plate;	
	m_henttype = src.m_henttype;
	m_syncit = src.m_syncit;
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
	m_Locked = src.m_Locked;
}

uint32_t HentView::GetHentId() const
{
	return m_Id;
}

const std::wstring& HentView::GetName() const
{
	return m_name;
}

void HentView:: SetName(const std::wstring& sName)
{
	m_name = sName;
}

const std::wstring&  HentView::GetAlias() const
{
	return m_alias;
}

void HentView:: SetAlias(const std::wstring& sAlias)
{
	m_alias = sAlias;
}

const std::wstring&  HentView::GetStreet() const
{
	return m_street;
}

void HentView:: SetStreet(const std::wstring& sStreet)
{
	m_street = sStreet;
}

const std::wstring&  HentView::GetPOBox() const
{
	return m_pobox;
}

void HentView:: SetPOBox(const std::wstring& sPOBox)
{
	m_pobox = sPOBox;
}


const std::wstring&  HentView::GetCity() const
{
	return m_city;
}

void HentView:: SetCity(const std::wstring& sCity)
{
	m_city = sCity;
}

const std::wstring&  HentView::GetZip() const
{
	return m_zip;
}

void HentView:: SetZip(const std::wstring& sZip)
{
	m_zip = sZip;
}


const std::wstring&  HentView::GetPhoneNo() const
{
	return m_phone;
}

void HentView:: SetPhoneNo(const std::wstring& sPhone)
{
	m_phone = sPhone;
}


const std::wstring&  HentView::GetFiscalNo() const
{
	return m_nip;
}

void HentView:: SetFiscalNo(const std::wstring& sFiscalNo)
{
	m_nip = sFiscalNo;
}

const HentNo&  HentView::GetHentNo() const
{
	return m_hentno;
}

void HentView:: SetHentNo(const HentNo& hentNo)
{
	m_hentno = hentNo;
}

const std::wstring&  HentView::GetWetNo() const
{
	return m_wetno;
}

void HentView:: SetWetNo(const std::wstring& sWetNo)
{
	m_wetno = sWetNo;
}

const std::wstring&  HentView::GetExtras() const
{
	return m_extras;
}

void HentView:: SetExtras(const std::wstring& sExtras)
{
	m_extras = sExtras;
}

const std::wstring&  HentView::GetPlateNo() const
{
	return m_plate;
}
	
void HentView:: SetPlateNo(const std::wstring& sPlate)
{
	m_plate = sPlate;
}

HentType HentView::GetHentType() const
{
	return m_henttype;
}

void HentView:: SetHentType(HentType hentType)
{
	m_henttype = hentType;
}

bool HentView::GetSyncIt() const
{
	return m_syncit;
}

void HentView:: SetSyncIt(bool bSyncIt)
{
	m_syncit = bSyncIt;
}

const std::wstring&  HentView::GetPESEL() const
{
	return m_pesel;
}

void HentView:: SetPESEL(const std::wstring& sPESEL)
{
	m_pesel = sPESEL;
}

const std::wstring&  HentView::GetREGON() const
{
	return m_regon;
}

void HentView:: SetREGON(const std::wstring& sREGON)
{
	m_regon = sREGON;
}

const std::wstring&  HentView::GetIdNo() const
{
	return m_idno;
}

void HentView:: SetIdNo(const std::wstring& sIdNo)
{
	m_idno = sIdNo;
}

const DateTime& HentView::GetIssueDate() const
{
	return m_issuedate;
}

void HentView:: SetIssueDate(const DateTime& IdIssueDate)
{
	m_issuedate = IdIssueDate;
}

const std::wstring&  HentView::GetIssuePost() const
{
	return m_issuepost;
}

void HentView:: SetIssuePost(const std::wstring& sIssuePost)
{
	m_issuepost = sIssuePost;
}


const AccountNo&  HentView::GetAccountNo() const
{
	return m_accountno;
}

void HentView:: SetAccountNo(const AccountNo& accountNo)
{
	m_accountno = accountNo;
}

const std::wstring&  HentView::GetBankName() const
{
	return m_bankname;
}

void HentView:: SetBankName(const std::wstring& sBankName)
{
	m_bankname = sBankName;
}

const std::wstring&  HentView::GetWetLicenceNo() const
{
	return m_wetlicenceno;
}

void HentView::SetWetLicenceNo(const std::wstring& sWetLicenceNo)
{
	m_wetlicenceno = sWetLicenceNo;
}

const std::wstring&  HentView::GetCellPhoneNo() const
{
	return m_cellphoneno;
}

void HentView::SetCellPhoneNo(const std::wstring& sCellPhoneNo)
{
	m_cellphoneno = sCellPhoneNo;
}

const std::wstring&  HentView::GetEmailAddress() const
{
	return m_emailaddress;
}

void HentView::SetEmailAddress(const std::wstring& sEmailAddress)
{
	m_emailaddress = sEmailAddress;
}

const Latitude& HentView::GetLatitude() const
{
	return m_latitude;
}

void HentView::SetLatitude(const Latitude& lati)
{
	m_latitude = lati;
}


const Longitude& HentView::GetLongitude() const
{
	return m_longitude;
}

void HentView::SetLongitude(const Longitude& longi)
{
	m_longitude = longi;
}

void HentView::SetLocked(bool bLocked)
{
	m_Locked = bLocked;
}

bool  HentView::IsLocked() const
{
	return m_Locked;
}


