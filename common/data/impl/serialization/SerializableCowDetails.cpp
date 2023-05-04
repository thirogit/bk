#include "stdafx.h"
#include "SerializableCowDetails.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableCowDetails::SerializableCowDetails(CowDetails& details) : m_details(details)
{
}

Decimal& SerializableCowDetails::Weight()
{
	return m_details.m_weight;
}

uint32_t& SerializableCowDetails::StockId()
{
	return m_details.m_stockId;
}

uint32_t& SerializableCowDetails:: ClassId()
{
	return m_details.m_classId;
}

CowNo& SerializableCowDetails::CowNumber()
{
	return m_details.m_cowno;
}

CowNo& SerializableCowDetails::MotherNo()
{
	return m_details.m_motherNo;
}

DateTime& SerializableCowDetails::BirthDate()
{
	return m_details.m_birthdate;
}

std::wstring& SerializableCowDetails::PassNo()
{
	return m_details.m_passno;
}

std::wstring& SerializableCowDetails::HealthCertNo()
{
	return m_details.m_healthcertno;
}

DateTime& SerializableCowDetails::PassDate()
{
	return m_details.m_passdate;
}
	
std::wstring& SerializableCowDetails::BirthPlace()
{
	return m_details.m_birthplace;
}

std::wstring& SerializableCowDetails::Extras()
{
	return m_details.m_extras;
}
	
uint32_t& SerializableCowDetails::FirstOwnerId()
{
	return m_details.m_fstownerId;
}

CowSex&	SerializableCowDetails::Sex()
{
	return m_details.m_sex;
}

NullDecimal& SerializableCowDetails::TermBuyPrice()
{
	return m_details.m_termbuyprice;
}

NullDecimal& SerializableCowDetails::TermSellPrice()
{
	return m_details.m_termsellprice;
}

NullDecimal& SerializableCowDetails::TermBuyWeight()
{
	return m_details.m_termbuyweight;
}

NullDecimal& SerializableCowDetails::TermSellWeight()
{
	return m_details.m_termsellweight;
}

uint32_t& SerializableCowDetails::TermBuyClassId()
{
	return m_details.m_termsellclassId;
}

uint32_t& SerializableCowDetails::TermSellClassId()
{
	return m_details.m_termsellclassId;
}
	
void SerializableCowDetails::serialize(SF::Archive &archive)
{
	archive & Weight();
	archive & StockId();	
	archive & ClassId();	
	archive & CowNumber();	
	archive & MotherNo();
	archive & BirthDate();
	archive & PassNo();
	archive & HealthCertNo();
	archive & PassDate();	
	archive & BirthPlace();
	archive & Extras();	
	archive & FirstOwnerId();
	archive & Sex();	
	archive & TermBuyPrice();
	archive & TermSellPrice();
	archive & TermBuyWeight();
	archive & TermSellWeight();
	archive & TermBuyClassId();
	archive & TermSellClassId();

	
}