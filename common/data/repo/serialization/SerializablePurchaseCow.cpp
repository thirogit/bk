#include "stdafx.h"
#include "SerializablePurchaseCow.h"
#include "../../types/serialization/TypesSerialization.h"

SerializablePurchaseCow::SerializablePurchaseCow(PurchaseCow* pCow) : m_pCow(pCow)
{
}

std::wstring& SerializablePurchaseCow::Id()
{
	return m_pCow->m_id;
}

Decimal& SerializablePurchaseCow::Weight()
{
	return m_pCow->m_Weight;
}

std::wstring& SerializablePurchaseCow::StockCd()
{
	return m_pCow->m_StockCode;
}

std::wstring& SerializablePurchaseCow::ClassCd()
{
	return m_pCow->m_ClassCode;
}

CowNo& SerializablePurchaseCow::CowNumber()
{
	return m_pCow->m_CowNo;
}

CowNo& SerializablePurchaseCow::MotherNo()
{
	return m_pCow->m_MotherNo;
}

DateTime& SerializablePurchaseCow::BirthDate()
{
	return m_pCow->m_BirthDt;
}

std::wstring& SerializablePurchaseCow::PassportNo()
{
	return m_pCow->m_PassportNo;
}

std::wstring& SerializablePurchaseCow::HealthCertNo()
{
	return m_pCow->m_HealthCertNo;
}

DateTime& SerializablePurchaseCow::PassportIssueDt()
{
	return m_pCow->m_PassportIssueDt;
}
	
std::wstring& SerializablePurchaseCow::BirthPlace()
{
	return m_pCow->m_BirthPlace;
}
	
HentNo& SerializablePurchaseCow::FirstOwnerNo()
{
	return m_pCow->m_FirstOwnerNo;
}

CowSex&	SerializablePurchaseCow::Sex()
{
	return m_pCow->m_Sex;
}

Decimal& SerializablePurchaseCow::Price()
{
	return m_pCow->m_Price;
}
	
void SerializablePurchaseCow::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & Weight();
	archive & StockCd();	
	archive & ClassCd();	
	archive & CowNumber();	
	archive & MotherNo();
	archive & BirthDate();
	archive & PassportNo();
	archive & HealthCertNo();
	archive & PassportIssueDt();	
	archive & BirthPlace();
	archive & FirstOwnerNo();
	archive & Sex();
	archive & Price();
}