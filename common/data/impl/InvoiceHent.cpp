#include "stdafx.h"
#include "InvoiceHent.h"
#include "../IIdable.h"
#include <utils/JsonUtils.h>

InvoiceHent::InvoiceHent() :  m_parentHentId(NULL_ID)
{
}

void InvoiceHent::CopyFrom(const InvoiceHent& src)
{
	CopyFrom(&src);
}

void InvoiceHent::CopyFrom(const IInvoiceHent* src)
{
	m_name = src->GetName();
	m_street = src->GetStreet();
	m_pobox = src->GetPOBox();
	m_city = src->GetCity();
	m_zip = src->GetZip();
	m_nip = src->GetFiscalNo();
	m_hentno = src->GetHentNo();
	m_pesel = src->GetPESEL();
	m_idno = src->GetIdNo();
	m_issuedate = src->GetIssueDate();
	m_issuepost = src->GetIssuePost();
	m_accountno = src->GetAccountNo();
	m_bankname = src->GetBankName();
	m_parentHentId = src->GetParentHentId();
}

void InvoiceHent::CopyFrom(const IHent* src)
{
	m_name = src->GetName();
	m_street = src->GetStreet();
	m_pobox = src->GetPOBox();
	m_city = src->GetCity();
	m_zip = src->GetZip();
	m_nip = src->GetFiscalNo();
	m_hentno = src->GetHentNo();
	m_pesel = src->GetPESEL();
	m_idno = src->GetIdNo();
	m_issuedate = src->GetIssueDate();
	m_issuepost = src->GetIssuePost();
	m_accountno = src->GetAccountNo();
	m_bankname = src->GetBankName();
	m_parentHentId = src->GetId();
}


const std::wstring& InvoiceHent::GetName() const
{
	return m_name;
}

void InvoiceHent:: SetName(const std::wstring& sName)
{
	m_name = sName;
}


const std::wstring&  InvoiceHent::GetStreet() const
{
	return m_street;
}

void InvoiceHent::SetStreet(const std::wstring& sStreet)
{
	m_street = sStreet;
}

const std::wstring&  InvoiceHent::GetPOBox() const
{
	return m_pobox;
}

void InvoiceHent::SetPOBox(const std::wstring& sPOBox)
{
	m_pobox = sPOBox;
}


const std::wstring&  InvoiceHent::GetCity() const
{
	return m_city;
}

void InvoiceHent::SetCity(const std::wstring& sCity)
{
	m_city = sCity;
}

const std::wstring&  InvoiceHent::GetZip() const
{
	return m_zip;
}

void InvoiceHent::SetZip(const std::wstring& sZip)
{
	m_zip = sZip;
}

const std::wstring&  InvoiceHent::GetFiscalNo() const
{
	return m_nip;
}

void InvoiceHent::SetFiscalNo(const std::wstring& sFiscalNo)
{
	m_nip = sFiscalNo;
}

const HentNo&  InvoiceHent::GetHentNo() const
{
	return m_hentno;
}

void InvoiceHent::SetHentNo(const HentNo& hentNo)
{
	m_hentno = hentNo;
}

const std::wstring&  InvoiceHent::GetPESEL() const
{
	return m_pesel;
}

void InvoiceHent::SetPESEL(const std::wstring& sPESEL)
{
	m_pesel = sPESEL;
}

const std::wstring&  InvoiceHent::GetIdNo() const
{
	return m_idno;
}

void InvoiceHent::SetIdNo(const std::wstring& sIdNo)
{
	m_idno = sIdNo;
}

const DateTime& InvoiceHent::GetIssueDate() const
{
	return m_issuedate;
}

void InvoiceHent::SetIssueDate(const DateTime& IdIssueDate)
{
	m_issuedate = IdIssueDate;
}

const std::wstring&  InvoiceHent::GetIssuePost() const
{
	return m_issuepost;
}

void InvoiceHent::SetIssuePost(const std::wstring& sIssuePost)
{
	m_issuepost = sIssuePost;
}

const AccountNo&  InvoiceHent::GetAccountNo() const
{
	return m_accountno;
}

void InvoiceHent::SetAccountNo(const AccountNo& accountNo)
{
	m_accountno = accountNo;
}

const std::wstring&  InvoiceHent::GetBankName() const
{
	return m_bankname;
}

void InvoiceHent::SetBankName(const std::wstring& sBankName)
{
	m_bankname = sBankName;
}

uint32_t InvoiceHent::GetParentHentId() const
{
	return m_parentHentId;
}

void InvoiceHent::SetParentHentId(uint32_t parentHentId)
{
	m_parentHentId = parentHentId;
}


static const wchar_t* hentname_attribute_name = L"hent_name";
static const wchar_t* city_attribute_name = L"city";
static const wchar_t* zipcode_attribute_name = L"zip_code";
static const wchar_t* street_attribute_name = L"street";
static const wchar_t* pobox_attribute_name = L"pobox";
static const wchar_t* hentno_attribute_name = L"hent_no";
static const wchar_t* fiscalno_attribute_name = L"fiscal_no";
static const wchar_t* personalno_attribute_name = L"personal_no";
static const wchar_t* personalidno_attribute_name = L"personal_id_no";
static const wchar_t* personalidissuedt_attribute_name = L"personal_id_issue_dt";
static const wchar_t* personalidissuepost_attribute_name = L"personal_id_issue_post";
static const wchar_t* accountno_attribute_name = L"account_no";
static const wchar_t* bankname_attribute_name = L"bank_name";
static const wchar_t* parenthentid_attribute_name = L"parent_hent_id";


InvoiceHent InvoiceHent::FromJson(const json::value& value)
{
	InvoiceHent hent;

	hent.SetName(value[hentname_attribute_name].as_string());
	hent.SetCity(value[city_attribute_name].as_string());
	hent.SetZip(value[zipcode_attribute_name].as_string());
	hent.SetStreet(value[street_attribute_name].as_string());
	hent.SetPOBox(value[pobox_attribute_name].as_string());
	hent.SetHentNo(HentNoFactory::FromString(value[hentno_attribute_name].as_string()));
	hent.SetFiscalNo(value[fiscalno_attribute_name].as_string());
	hent.SetPESEL(value[personalno_attribute_name].as_string());
	hent.SetIdNo(value[personalidno_attribute_name].as_string());
	hent.SetIssueDate(json_DayDate(value[personalidissuedt_attribute_name]));
	hent.SetIssuePost(value[personalidissuepost_attribute_name].as_string());
	hent.SetAccountNo(AccountNoFactory::FromString(value[accountno_attribute_name].as_string()));
	hent.SetBankName(value[bankname_attribute_name].as_string());
	hent.SetParentHentId(value[parenthentid_attribute_name].as_integer());


	return hent;
}

json::value InvoiceHent::ToJson() const
{
	json::value result;

	result[hentname_attribute_name] = json::value(GetName());
	result[city_attribute_name] = json::value(GetCity());
	result[zipcode_attribute_name] = json::value(GetZip());
	result[street_attribute_name] = json::value(GetStreet());
	result[pobox_attribute_name] = json::value(GetPOBox());
	result[hentno_attribute_name] = json::value(GetHentNo().ToString());
	result[fiscalno_attribute_name] = json::value(GetFiscalNo());
	result[personalno_attribute_name] = json::value(GetPESEL());
	result[personalidno_attribute_name] = json::value(GetIdNo());
	result[personalidissuepost_attribute_name] = json::value(GetIssuePost());
	result[accountno_attribute_name] = json::value(GetAccountNo().ToString());
	result[bankname_attribute_name] = json::value(GetBankName());
	result[parenthentid_attribute_name] = json::value(GetParentHentId());
	result[personalidissuedt_attribute_name] = json_DayDate(GetIssueDate());
	return result;
}