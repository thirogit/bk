#include "stdafx.h"
#include "SerializableClass.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableClass::SerializableClass(Class* pClass) : m_pClass(pClass)
{
}

	
uint32_t& SerializableClass::Id()
{
	return m_pClass->m_classid;
}


std::wstring& 	SerializableClass::ClassName()
{
	return m_pClass->m_classname;
}

std::wstring& 	SerializableClass::ClassCode()
{
	return m_pClass->m_classcode;
}

NullDecimal&	SerializableClass::MyPerKgPrice()
{
	return m_pClass->m_myperkgprice;
}

CowSex&			SerializableClass::PredefSex()
{
	return m_pClass->m_predefsex;
}

uint32_t&		SerializableClass::RuleId()
{
	return m_pClass->m_ruleId;
}

void SerializableClass::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & ClassName();	
	archive & ClassCode();	
	archive & MyPerKgPrice();	
	archive & PredefSex();	
	archive & RuleId();
}