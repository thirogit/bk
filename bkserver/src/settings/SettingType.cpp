#include "stdafx.h"
#include "SettingType.h"

SettingType::_SettingType::_SettingType() : m_collection(false), m_association(false), m_propertyType(ValueType_None), m_keyType(ValueType_None)
{
}

//#################################################################################################

SettingType::SettingType()
{
	MakeStr();
}

SettingType::SettingType(const SettingType& src)
{
	m_Value = src.m_Value;
	MakeStr();
}

SettingType& SettingType::operator=(const SettingType& right)
{
	m_Value = right.m_Value;
	MakeStr();
	return *this;
}


#ifdef _DEBUG

std::wstring SettingType::ValueTypeToStr(ValueType type)
{
	switch (type)
	{
	case ValueType_Integer:
		return L"int";
	case ValueType_String:
		return L"string";
	case ValueType_Decimal:
		return L"decimal";	
	}
	return L"";
}
#endif


void SettingType::MakeStr()
{
#ifdef _DEBUG
	m_str = L"";
	if (!IsNull())
	{
		if (m_Value->m_collection)
		{
			m_str = L"vector<" + ValueTypeToStr(m_Value->m_propertyType) + L">";
		}
		else if (m_Value->m_association)
		{
			m_str = L"map<" + ValueTypeToStr(m_Value->m_keyType) + L"," + ValueTypeToStr(m_Value->m_propertyType) + L">";
		}
	}
#endif
}


void SettingType::MakeNull()
{
	m_Value.reset();
	MakeStr();
}

bool SettingType::IsNull() const
{
	return !m_Value.is_initialized();
}

void SettingType::EnsureValue()
{
	if (!m_Value.is_initialized())
	{
		m_Value = _SettingType();
	}
}

void SettingType::SetIsCollection(bool bCollection)
{
	EnsureValue();
	m_Value->m_collection = bCollection;
	MakeStr();
}

void SettingType::SetIsAssociation(bool bAssociation)
{
	EnsureValue();
	m_Value->m_association = bAssociation;
	MakeStr();
}

void SettingType::SetPropertyType(ValueType type)
{
	EnsureValue();
	m_Value->m_propertyType = type;
	MakeStr();
}

void SettingType::SetKeyType(ValueType type)
{
	EnsureValue();
	m_Value->m_keyType = type;
	MakeStr();
}

bool SettingType::GetIsCollection() const
{
	if (IsNull())
		return false;
	return m_Value->m_collection;
}

bool SettingType::GetIsAssociation() const
{
	if (IsNull())
		return false;

	return m_Value->m_association;
}

ValueType SettingType::GetPropertyType() const
{
	if (IsNull())
		return ValueType_None;

	return m_Value->m_propertyType;
}

ValueType SettingType::GetKeyType() const
{
	if (IsNull())
		return ValueType_None;
	return m_Value->m_keyType;
}
	
