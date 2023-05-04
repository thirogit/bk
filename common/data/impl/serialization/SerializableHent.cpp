#include "stdafx.h"
#include "SerializableHent.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableHent::SerializableHent(Hent* pHent) : m_pHent(pHent)
{
}

std::wstring& SerializableHent::Name()
{
	return m_pHent->m_name;
}

std::wstring& SerializableHent::City()
{
	return m_pHent->m_city;
}

std::wstring& SerializableHent::ZipCode()
{
	return m_pHent->m_zip;
}

std::wstring& SerializableHent::Street()
{
	return m_pHent->m_street;
}

std::wstring& SerializableHent::POBox()
{
	return m_pHent->m_pobox;
}

HentNo& SerializableHent::HentNumber()
{
	return m_pHent->m_hentno;
}
	
std::wstring& SerializableHent::FiscalNo()
{
	return m_pHent->m_nip;
}

std::wstring& SerializableHent::REGON()
{
	return m_pHent->m_regon;
}

std::wstring& SerializableHent::Bank()
{
	return m_pHent->m_bankname;
}

AccountNo& SerializableHent::AccountNumber()
{
	return m_pHent->m_accountno;
}

std::wstring& SerializableHent::WetIdNo()
{
	return m_pHent->m_wetno;
}

std::wstring& SerializableHent::WetLicNo()
{
	return m_pHent->m_wetlicenceno;
}

std::wstring& SerializableHent::PhoneNo()
{
	return m_pHent->m_phone;
}
	
uint32_t& SerializableHent::Id()
{
	return m_pHent->m_Id;
}

std::wstring& SerializableHent::Alias()
{
	return m_pHent->m_alias;
}

std::wstring& SerializableHent::Extras()
{
	return m_pHent->m_extras;
}

std::wstring& SerializableHent::PlateNo()
{
	return m_pHent->m_plate;
}

HentType& SerializableHent::TypeOfHent()
{
	return m_pHent->m_henttype;
}

bool& SerializableHent::GetSyncIt()
{
	return m_pHent->m_syncit;
}

std::wstring& SerializableHent::PersonalNo()
{
	return m_pHent->m_pesel;
}

std::wstring& SerializableHent::PersonalIdNo()
{
	return m_pHent->m_idno;
}

DateTime& SerializableHent::IssueDate()
{
	return m_pHent->m_issuedate;
}

std::wstring& SerializableHent::IssuePost()
{
	return m_pHent->m_issuepost;
}

std::wstring& SerializableHent::BankName()
{
	return m_pHent->m_bankname;
}

std::wstring& SerializableHent::CellPhoneNo()
{
	return m_pHent->m_cellphoneno;
}

std::wstring& SerializableHent::EmailAddress()
{
	return m_pHent->m_emailaddress;
}


void SerializableHent::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & Name();
	archive & City();
	archive & ZipCode();
	archive & Street();
	archive & POBox();	
	archive & FiscalNo();
	archive & REGON();
	archive & Bank();
	archive & AccountNumber();
	archive & WetIdNo();
	archive & WetLicNo();
	archive & PhoneNo();		
	archive & Alias();		
	archive & HentNumber();	
	archive & Extras();
	archive & PlateNo();
	archive & TypeOfHent();
	archive & GetSyncIt();
	archive & PersonalNo();
	archive & PersonalIdNo();
	archive & IssueDate();
	archive & IssuePost();
	archive & BankName();
	archive & CellPhoneNo();
	archive & EmailAddress();
	//archive & Latitude();
	//archive & Longitude();
}