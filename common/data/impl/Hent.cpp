#include "stdafx.h"
#include "Hent.h"
#include <utils/JsonUtils.h>


Hent::Hent(uint32_t hentId) :  m_Id(hentId),m_henttype(HentType_None),m_syncit(false)
{
}

Hent::~Hent()
{

}

void Hent::CopyFrom(const IHent* src)
{
	m_name = src->GetName();
	m_alias = src->GetAlias();
	m_street = src->GetStreet();
	m_pobox = src->GetPOBox();
	m_city = src->GetCity();
	m_zip = src->GetZip();
	m_phone = src->GetPhoneNo();
	m_nip = src->GetFiscalNo();
	m_hentno = src->GetHentNo();
	m_wetno = src->GetWetNo();
	m_extras = src->GetExtras();
	m_plate = src->GetPlateNo();	
	m_henttype = src->GetHentType();
	m_pesel = src->GetPESEL();
	m_regon = src->GetREGON();
	m_idno = src->GetIdNo();
	m_issuedate = src->GetIssueDate();
	m_issuepost = src->GetIssuePost();
	m_accountno = src->GetAccountNo();
	m_bankname = src->GetBankName();
	m_wetlicenceno = src->GetWetLicenceNo();
	m_cellphoneno = src->GetCellPhoneNo();
	m_emailaddress = src->GetEmailAddress();
	m_latitude = src->GetLatitude();
	m_longitude = src->GetLongitude();
	m_syncit = src->GetSyncIt();
}

uint32_t Hent::GetId() const
{
	return m_Id;
}

const std::wstring& Hent::GetName() const
{
	return m_name;
}

void Hent:: SetName(const std::wstring& sName)
{
	m_name = sName;
}

const std::wstring&  Hent::GetAlias() const
{
	return m_alias;
}

void Hent:: SetAlias(const std::wstring& sAlias)
{
	m_alias = sAlias;
}

const std::wstring&  Hent::GetStreet() const
{
	return m_street;
}

void Hent:: SetStreet(const std::wstring& sStreet)
{
	m_street = sStreet;
}

const std::wstring&  Hent::GetPOBox() const
{
	return m_pobox;
}

void Hent:: SetPOBox(const std::wstring& sPOBox)
{
	m_pobox = sPOBox;
}


const std::wstring&  Hent::GetCity() const
{
	return m_city;
}

void Hent:: SetCity(const std::wstring& sCity)
{
	m_city = sCity;
}

const std::wstring&  Hent::GetZip() const
{
	return m_zip;
}

void Hent:: SetZip(const std::wstring& sZip)
{
	m_zip = sZip;
}


const std::wstring&  Hent::GetPhoneNo() const
{
	return m_phone;
}

void Hent:: SetPhoneNo(const std::wstring& sPhone)
{
	m_phone = sPhone;
}


const std::wstring&  Hent::GetFiscalNo() const
{
	return m_nip;
}

void Hent:: SetFiscalNo(const std::wstring& sFiscalNo)
{
	m_nip = sFiscalNo;
}


bool Hent::HasFiscalNo() const
{
	return !m_nip.empty();
}

const HentNo&  Hent::GetHentNo() const
{
	return m_hentno;
}

void Hent:: SetHentNo(const HentNo& hentNo)
{
	m_hentno = hentNo;
}

const std::wstring&  Hent::GetWetNo() const
{
	return m_wetno;
}

void Hent:: SetWetNo(const std::wstring& sWetNo)
{
	m_wetno = sWetNo;
}

bool Hent::HasWetNo() const
{
	return !m_wetno.empty();
}

const std::wstring&  Hent::GetExtras() const
{
	return m_extras;
}

void Hent:: SetExtras(const std::wstring& sExtras)
{
	m_extras = sExtras;
}




const std::wstring&  Hent::GetPlateNo() const
{
	return m_plate;
}
	
void Hent:: SetPlateNo(const std::wstring& sPlate)
{
	m_plate = sPlate;
}


HentType Hent::GetHentType() const
{
	return m_henttype;
}

void Hent:: SetHentType(HentType hentType)
{
	m_henttype = hentType;
}

bool Hent::GetSyncIt() const
{
	return m_syncit;
}

void Hent:: SetSyncIt(bool bSyncIt)
{
	m_syncit = bSyncIt;
}

const std::wstring&  Hent::GetPESEL() const
{
	return m_pesel;
}

void Hent:: SetPESEL(const std::wstring& sPESEL)
{
	m_pesel = sPESEL;
}

bool Hent::HasPESEL() const
{
	return !m_pesel.empty();
}

const std::wstring&  Hent::GetREGON() const
{
	return m_regon;
}

void Hent:: SetREGON(const std::wstring& sREGON)
{
	m_regon = sREGON;
}

bool Hent::HasREGON() const
{
	return !m_regon.empty();
}

const std::wstring&  Hent::GetIdNo() const
{
	return m_idno;
}

void Hent:: SetIdNo(const std::wstring& sIdNo)
{
	m_idno = sIdNo;
}

const DateTime& Hent::GetIssueDate() const
{
	return m_issuedate;
}

void Hent:: SetIssueDate(const DateTime& IdIssueDate)
{
	m_issuedate = IdIssueDate;
}

const std::wstring&  Hent::GetIssuePost() const
{
	return m_issuepost;
}

void Hent:: SetIssuePost(const std::wstring& sIssuePost)
{
	m_issuepost = sIssuePost;
}


const AccountNo&  Hent::GetAccountNo() const
{
	return m_accountno;
}

void Hent:: SetAccountNo(const AccountNo& accountNo)
{
	m_accountno = accountNo;
}

const std::wstring&  Hent::GetBankName() const
{
	return m_bankname;
}

void Hent:: SetBankName(const std::wstring& sBankName)
{
	m_bankname = sBankName;
}

const std::wstring&  Hent::GetWetLicenceNo() const
{
	return m_wetlicenceno;
}

void Hent::SetWetLicenceNo(const std::wstring& sWetLicenceNo)
{
	m_wetlicenceno = sWetLicenceNo;
}


bool Hent::HasWetLicenceNo() const
{
	return !m_wetlicenceno.empty();
}

const std::wstring&  Hent::GetCellPhoneNo() const
{
	return m_cellphoneno;
}

void Hent::SetCellPhoneNo(const std::wstring& sCellPhoneNo)
{
	m_cellphoneno = sCellPhoneNo;
}

const std::wstring&  Hent::GetEmailAddress() const
{
	return m_emailaddress;
}

void Hent::SetEmailAddress(const std::wstring& sEmailAddress)
{
	m_emailaddress = sEmailAddress;
}

const Latitude& Hent::GetLatitude() const
{
	return m_latitude;
}

void Hent::SetLatitude(const Latitude& lati)
{
	m_latitude = lati;
}


const Longitude& Hent::GetLongitude() const
{
	return m_longitude;
}

void Hent::SetLongitude(const Longitude& longi)
{
	m_longitude = longi;
}



static const wchar_t* _id_attribute_name = L"_id";
static const wchar_t* hentname_attribute_name = L"hent_name";
static const wchar_t* alias_attribute_name = L"alias";
static const wchar_t* city_attribute_name = L"city";
static const wchar_t* zipcode_attribute_name = L"zip_code";
static const wchar_t* street_attribute_name = L"street";
static const wchar_t* pobox_attribute_name = L"pobox";
static const wchar_t* hentno_attribute_name = L"hent_no";
static const wchar_t* fiscalno_attribute_name = L"fiscal_no";
static const wchar_t* statsno_attribute_name = L"stats_no";
static const wchar_t* wetidno_attribute_name = L"wet_id_no";
static const wchar_t* wetlicno_attribute_name = L"wet_lic_no";
static const wchar_t* phoneno_attribute_name = L"phone_no";
static const wchar_t* extras_attribute_name = L"extras";
static const wchar_t* henttype_attribute_name = L"hent_type";
static const wchar_t* personalno_attribute_name = L"personal_no";
static const wchar_t* plateno_attribute_name = L"plate_no";
static const wchar_t* personalidno_attribute_name = L"personal_id_no";
static const wchar_t* personalidissuedt_attribute_name = L"personal_id_issue_dt";
static const wchar_t* personalidissuepost_attribute_name = L"personal_id_issue_post";
static const wchar_t* accountno_attribute_name = L"account_no";
static const wchar_t* bankname_attribute_name = L"bank_name";
static const wchar_t* cellphoneno_attribute_name = L"cellphone_no";
static const wchar_t* emailaddress_attribute_name = L"email_address";
static const wchar_t* longitude_attribute_name = L"longitude";
static const wchar_t* latitude_attribute_name = L"latitude";
static const wchar_t* syncit_attribute_name = L"sync_it";


Hent Hent::FromJson(const json::value& value)
{
	Hent hent(value[_id_attribute_name].as_integer());

	hent.SetName(value[hentname_attribute_name].as_string());
	hent.SetCity(value[city_attribute_name].as_string());
	hent.SetZip(value[zipcode_attribute_name].as_string());
	hent.SetStreet(value[street_attribute_name].as_string());
	hent.SetPOBox(value[pobox_attribute_name].as_string());
	hent.SetHentNo(HentNoFactory::FromString(value[hentno_attribute_name].as_string()));
	hent.SetFiscalNo(value[fiscalno_attribute_name].as_string());
	hent.SetREGON(value[statsno_attribute_name].as_string());
	hent.SetWetNo(value[wetidno_attribute_name].as_string());
	hent.SetWetLicenceNo(value[wetlicno_attribute_name].as_string());
	hent.SetPhoneNo(value[phoneno_attribute_name].as_string());

	hent.SetAlias(value[alias_attribute_name].as_string());
	hent.SetExtras(value[extras_attribute_name].as_string());
	hent.SetPlateNo(value[plateno_attribute_name].as_string());
	hent.SetHentType(json_HentType(value[henttype_attribute_name]));
	hent.SetPESEL(value[personalno_attribute_name].as_string());
	hent.SetIdNo(value[personalidno_attribute_name].as_string());
	hent.SetIssueDate(json_DayDate(value[personalidissuedt_attribute_name]));
	hent.SetIssuePost(value[personalidissuepost_attribute_name].as_string());
	hent.SetAccountNo(AccountNoFactory::FromString(value[accountno_attribute_name].as_string()));
	hent.SetBankName(value[bankname_attribute_name].as_string());
	hent.SetCellPhoneNo(value[cellphoneno_attribute_name].as_string());
	hent.SetEmailAddress(value[emailaddress_attribute_name].as_string());
	hent.SetLatitude(json_Latitude(value[latitude_attribute_name]));
	hent.SetLongitude(json_Longitude(value[longitude_attribute_name]));
	hent.SetSyncIt(json_bool(value[syncit_attribute_name]));

	return hent;
}

json::value Hent::ToJson() const
{
	json::value result;

	result[hentname_attribute_name] = json::value(GetName());
	result[city_attribute_name] = json::value(GetCity());
	result[zipcode_attribute_name] = json::value(GetZip());
	result[street_attribute_name] = json::value(GetStreet());
	result[pobox_attribute_name] = json::value(GetPOBox());
	result[hentno_attribute_name] = json::value(GetHentNo().ToString());
	result[fiscalno_attribute_name] = json::value(GetFiscalNo());
	result[statsno_attribute_name] = json::value(GetREGON());
	result[wetidno_attribute_name] = json::value(GetWetNo());
	result[wetlicno_attribute_name] = json::value(GetWetLicenceNo());
	result[phoneno_attribute_name] = json::value(GetPhoneNo());
	result[alias_attribute_name] = json::value(GetAlias());
	result[extras_attribute_name] = json::value(GetExtras());
	result[plateno_attribute_name] = json::value(GetPlateNo());
	result[henttype_attribute_name] = json::value(json_HentType(GetHentType()));
	result[personalno_attribute_name] = json::value(GetPESEL());
	result[statsno_attribute_name] = json::value(GetREGON());
	result[personalidno_attribute_name] = json::value(GetIdNo());
	result[personalidissuepost_attribute_name] = json::value(GetIssuePost());
	result[accountno_attribute_name] = json::value(GetAccountNo().ToString());
	result[bankname_attribute_name] = json::value(GetBankName());
	result[cellphoneno_attribute_name] = json::value(GetCellPhoneNo());
	result[emailaddress_attribute_name] = json::value(GetEmailAddress());
	result[latitude_attribute_name] = json::value(json_Latitude(GetLatitude()));
	result[longitude_attribute_name] = json::value(json_Longitude(GetLongitude()));
	result[syncit_attribute_name] = json::value(json_bool(GetSyncIt()));
	result[_id_attribute_name] = json::value(GetId());
	result[personalidissuedt_attribute_name] = json_DayDate(GetIssueDate());
	return result;
}

