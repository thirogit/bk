#include "stdafx.h"
#include "SerializableInvoiceHent.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableInvoiceHent::SerializableInvoiceHent(InvoiceHent& hent) : m_Hent(hent)
{
}

std::wstring& SerializableInvoiceHent::Name()
{
	return m_Hent.m_name;
}

std::wstring& SerializableInvoiceHent::City()
{
	return m_Hent.m_city;
}

std::wstring& SerializableInvoiceHent::ZipCode()
{
	return m_Hent.m_zip;
}

std::wstring& SerializableInvoiceHent::Street()
{
	return m_Hent.m_street;
}

std::wstring& SerializableInvoiceHent::POBox()
{
	return m_Hent.m_pobox;
}

HentNo& SerializableInvoiceHent::HentNumber()
{
	return m_Hent.m_hentno;
}
	
std::wstring& SerializableInvoiceHent::FiscalNumber()
{
	return m_Hent.m_nip;
}

AccountNo& SerializableInvoiceHent::AccountNumber()
{
	return m_Hent.m_accountno;
}

std::wstring& SerializableInvoiceHent::PersonalNo()
{
	return m_Hent.m_pesel;
}

std::wstring& SerializableInvoiceHent::PersonalIdNo()
{
	return m_Hent.m_idno;
}

DateTime& SerializableInvoiceHent::IssueDate()
{
	return m_Hent.m_issuedate;
}

std::wstring& SerializableInvoiceHent::IssuePost()
{
	return m_Hent.m_issuepost;
}

std::wstring& SerializableInvoiceHent::BankName()
{
	return m_Hent.m_bankname;
}

uint32_t& SerializableInvoiceHent::ParentHentId()
{
	return m_Hent.m_parentHentId;
}

void SerializableInvoiceHent::serialize(SF::Archive &archive)
{
	archive & ParentHentId();
	archive & Name();
	archive & City();
	archive & ZipCode();
	archive & Street();
	archive & POBox();	
	archive & FiscalNumber();	
	archive & AccountNumber();			
	archive & HentNumber();		
	archive & PersonalNo();
	archive & PersonalIdNo();
	archive & IssueDate();
	archive & IssuePost();
	archive & BankName();
}