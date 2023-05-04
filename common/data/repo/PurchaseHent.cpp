#include "stdafx.h"
#include "PurchaseHent.h"

PurchaseHent::PurchaseHent(const std::wstring& id) : m_henttype(HentType_None), m_id(id)
{
}

PurchaseHent::PurchaseHent(const PurchaseHent& src) : m_id(src.GetId())
{
	CopyFrom(src);
}

void PurchaseHent::CopyFrom(const PurchaseHent& src)
{
	m_name = src.m_name;
	m_alias = src.m_alias;
	m_street = src.m_street;
	m_pobox = src.m_pobox;
	m_city = src.m_city;
	m_zip = src.m_zip;
	m_phone = src.m_phone;
	m_fiscalNo = src.m_fiscalNo;
	m_hentno = src.m_hentno;
	m_wetno = src.m_wetno;
	m_extras = src.m_extras;
	m_plate = src.m_plate;
	m_henttype = src.m_henttype;
	m_personalNo = src.m_personalNo;
	m_statsNo = src.m_statsNo;
	m_personalIdNo = src.m_personalIdNo;
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

void PurchaseHent::CopyFrom(const IPurchaseHent* hent)
{
	m_name = hent->GetName();
	m_alias = hent->GetAlias();
	m_street = hent->GetStreet();
	m_pobox = hent->GetPOBox();
	m_city = hent->GetCity();
	m_zip = hent->GetZip();
	m_phone = hent->GetPhoneNo();
	m_fiscalNo = hent->GetFiscalNo();
	m_hentno = hent->GetHentNo();
	m_wetno = hent->GetWetNo();
	m_extras = hent->GetExtras();
	m_plate = hent->GetPlateNo();
	m_henttype = hent->GetHentType();
	m_personalNo = hent->GetPersonalNo();
	m_statsNo = hent->GetStatsNo();
	m_personalIdNo = hent->GetPersonalIdNo();
	m_issuedate = hent->GetIssueDate();
	m_issuepost = hent->GetIssuePost();
	m_accountno = hent->GetAccountNo();
	m_bankname = hent->GetBankName();
	m_wetlicenceno = hent->GetWetLicenceNo();
	m_cellphoneno = hent->GetCellPhoneNo();
	m_emailaddress = hent->GetEmailAddress();
	m_latitude = hent->GetLatitude();
	m_longitude = hent->GetLongitude();
}

void PurchaseHent::CopyFrom(const IHent* hent)
{
	m_name = hent->GetName();
	m_alias = hent->GetAlias();
	m_street = hent->GetStreet();
	m_pobox = hent->GetPOBox();
	m_city = hent->GetCity();
	m_zip = hent->GetZip();
	m_phone = hent->GetPhoneNo();
	m_fiscalNo = hent->GetFiscalNo();
	m_hentno = hent->GetHentNo();
	m_wetno = hent->GetWetNo();
	m_extras = hent->GetExtras();
	m_plate = hent->GetPlateNo();
	m_henttype = hent->GetHentType();
	m_personalNo = hent->GetPESEL();
	m_statsNo = hent->GetREGON();
	m_personalIdNo = hent->GetIdNo();
	m_issuedate = hent->GetIssueDate();
	m_issuepost = hent->GetIssuePost();
	m_accountno = hent->GetAccountNo();
	m_bankname = hent->GetBankName();
	m_wetlicenceno = hent->GetWetLicenceNo();
	m_cellphoneno = hent->GetCellPhoneNo();
	m_emailaddress = hent->GetEmailAddress();
	m_latitude = hent->GetLatitude();
	m_longitude = hent->GetLongitude();
}

const std::wstring& PurchaseHent::GetId() const
{
	return m_id;
}

const std::wstring& PurchaseHent::GetName() const
{
	return m_name;
}

void PurchaseHent::SetName(const std::wstring& hentName)
{
	m_name = hentName;
}


const std::wstring& PurchaseHent::GetAlias() const
{
	return m_alias;
}

void PurchaseHent::SetAlias(const std::wstring& hentAlias)
{
	m_alias = hentAlias;
}

const std::wstring& PurchaseHent::GetStreet() const
{
	return m_street;
}

void PurchaseHent::SetStreet(const std::wstring& hentStreet)
{
	m_street = hentStreet;
}

const std::wstring& PurchaseHent::GetPOBox() const
{
	return m_pobox;
}

void PurchaseHent::SetPOBox(const std::wstring& hentPOBox)
{
	m_pobox = hentPOBox;
}

const std::wstring& PurchaseHent::GetCity() const
{
	return m_city;
}

void PurchaseHent::SetCity(const std::wstring& hentCity)
{
	m_city = hentCity;
}

const std::wstring& PurchaseHent::GetZip() const
{
	return m_zip;
}

void PurchaseHent::SetZip(const std::wstring& hentZip)
{
	m_zip = hentZip;
}

const std::wstring& PurchaseHent::GetPhoneNo() const
{
	return m_phone;
}

void PurchaseHent::SetPhoneNo(const std::wstring& hentPhone)
{
	m_phone = hentPhone;
}

const std::wstring& PurchaseHent::GetFiscalNo() const
{
	return m_fiscalNo;
}

void PurchaseHent::SetFiscalNo(const std::wstring& hentNIP)
{
	m_fiscalNo = hentNIP;
}

const HentNo& PurchaseHent::GetHentNo() const
{
	return m_hentno;
}

void PurchaseHent::SetHentNo(const HentNo& hentFarmNo)
{
	m_hentno = hentFarmNo;
}

const std::wstring& PurchaseHent::GetWetNo() const
{
	return m_wetno;
}

void PurchaseHent::SetWetNo(const std::wstring& hentWetNo)
{
	m_wetno = hentWetNo;
}

const std::wstring& PurchaseHent::GetExtras() const
{
	return m_extras;
}

void PurchaseHent::SetExtras(const std::wstring& hentExtras)
{
	m_extras = hentExtras;
}

const std::wstring& PurchaseHent::GetPlateNo() const
{
	return m_plate;
}

void PurchaseHent::SetPlateNo(const std::wstring& hentPlate)
{
	m_plate = hentPlate;
}

HentType PurchaseHent::GetHentType() const
{
	return m_henttype;
}

void PurchaseHent::SetHentType(HentType hentType)
{
	m_henttype = hentType;
}

const std::wstring& PurchaseHent::GetPersonalNo() const
{
	return m_personalNo;
}

void PurchaseHent::SetPersonalNo(const std::wstring& hentPESEL)
{
	m_personalNo = hentPESEL;
}

const std::wstring& PurchaseHent::GetStatsNo() const
{
	return m_statsNo;
}

void PurchaseHent::SetStatsNo(const std::wstring& hentREGON)
{
	m_statsNo = hentREGON;
}

const std::wstring& PurchaseHent::GetPersonalIdNo() const
{
	return m_personalIdNo;
}

void PurchaseHent::SetPersonalIdNo(const std::wstring& hentIdNo)
{
	m_personalIdNo = hentIdNo;
}

const DateTime& PurchaseHent::GetIssueDate() const
{
	return m_issuedate;
}

void PurchaseHent::SetIssueDate(const DateTime& hentIdIssueDate)
{
	m_issuedate = hentIdIssueDate;
}

const std::wstring& PurchaseHent::GetIssuePost() const
{
	return m_issuepost;
}

void PurchaseHent::SetIssuePost(const std::wstring& hentIssuePost)
{
	m_issuepost = hentIssuePost;
}

const AccountNo& PurchaseHent::GetAccountNo() const
{
	return m_accountno;
}

void PurchaseHent::SetAccountNo(const AccountNo& hentAccountNo)
{
	m_accountno = hentAccountNo;
}

const std::wstring& PurchaseHent::GetBankName() const
{
	return m_bankname;
}

void PurchaseHent::SetBankName(const std::wstring& hentBankName)
{
	m_bankname = hentBankName;
}

const std::wstring& PurchaseHent::GetWetLicenceNo() const
{
	return m_wetlicenceno;
}

void PurchaseHent::SetWetLicenceNo(const std::wstring& sWetLicenceNo)
{
	m_wetlicenceno = sWetLicenceNo;
}

const std::wstring& PurchaseHent::GetCellPhoneNo() const
{
	return m_cellphoneno;
}

void PurchaseHent::SetCellPhoneNo(const std::wstring& sCellPhoneNo)
{
	m_cellphoneno = sCellPhoneNo;
}

const std::wstring& PurchaseHent::GetEmailAddress() const
{
	return m_emailaddress;
}

void PurchaseHent::SetEmailAddress(const std::wstring& sEmailAddress)
{
	m_emailaddress = sEmailAddress;
}

const Latitude& PurchaseHent::GetLatitude() const
{
	return m_latitude;
}

void PurchaseHent::SetLatitude(const Latitude& lati)
{
	m_latitude = lati;

}

const Longitude& PurchaseHent::GetLongitude() const
{
	return m_longitude;
}

void PurchaseHent::SetLongitude(const Longitude& longi)
{
	m_longitude = longi;

}