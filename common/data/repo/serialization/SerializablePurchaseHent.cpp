#include "stdafx.h"
#include "SerializablePurchaseHent.h"
#include "../../types/serialization/TypesSerialization.h"

SerializablePurchaseHent::SerializablePurchaseHent(PurchaseHent* pHent) : m_pHent(pHent)
{
}

std::wstring& SerializablePurchaseHent::Id()
{
	return m_pHent->m_id;
}

std::wstring& SerializablePurchaseHent::Name()
{
	return m_pHent->m_name;
}

std::wstring& SerializablePurchaseHent::City()
{
	return m_pHent->m_city;
}

std::wstring& SerializablePurchaseHent::ZipCode()
{
	return m_pHent->m_zip;
}

std::wstring& SerializablePurchaseHent::Street()
{
	return m_pHent->m_street;
}

std::wstring& SerializablePurchaseHent::POBox()
{
	return m_pHent->m_pobox;
}

HentNo& SerializablePurchaseHent::HentNumber()
{
	return m_pHent->m_hentno;
}
	
std::wstring& SerializablePurchaseHent::FiscalNo()
{
	return m_pHent->m_fiscalNo;
}

std::wstring& SerializablePurchaseHent::StatsNo()
{
	return m_pHent->m_statsNo;
}

std::wstring& SerializablePurchaseHent::Bank()
{
	return m_pHent->m_bankname;
}

AccountNo& SerializablePurchaseHent::AccountNumber()
{
	return m_pHent->m_accountno;
}

std::wstring& SerializablePurchaseHent::WetIdNo()
{
	return m_pHent->m_wetno;
}

std::wstring& SerializablePurchaseHent::WetLicNo()
{
	return m_pHent->m_wetlicenceno;
}

std::wstring& SerializablePurchaseHent::PhoneNo()
{
	return m_pHent->m_phone;
}
	
std::wstring& SerializablePurchaseHent::Alias()
{
	return m_pHent->m_alias;
}

std::wstring& SerializablePurchaseHent::Extras()
{
	return m_pHent->m_extras;
}

std::wstring& SerializablePurchaseHent::PlateNo()
{
	return m_pHent->m_plate;
}

HentType& SerializablePurchaseHent::TypeOfHent()
{
	return m_pHent->m_henttype;
}


std::wstring& SerializablePurchaseHent::PersonalNo()
{
	return m_pHent->m_personalNo;
}

std::wstring& SerializablePurchaseHent::PersonalIdNo()
{
	return m_pHent->m_personalIdNo;
}

DateTime& SerializablePurchaseHent::IssueDate()
{
	return m_pHent->m_issuedate;
}

std::wstring& SerializablePurchaseHent::IssuePost()
{
	return m_pHent->m_issuepost;
}

std::wstring& SerializablePurchaseHent::BankName()
{
	return m_pHent->m_bankname;
}

std::wstring& SerializablePurchaseHent::CellPhoneNo()
{
	return m_pHent->m_cellphoneno;
}

std::wstring& SerializablePurchaseHent::EmailAddress()
{
	return m_pHent->m_emailaddress;
}


void SerializablePurchaseHent::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & Name();
	archive & City();
	archive & ZipCode();
	archive & Street();
	archive & POBox();	
	archive & FiscalNo();
	archive & StatsNo();
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