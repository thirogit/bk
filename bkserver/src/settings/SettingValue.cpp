#include "stdafx.h"
#include "SettingValue.h"


SettingValue::SettingValue() : m_order(0)
{
}

SettingValue::SettingValue(const std::wstring& sValue) : m_value(sValue),m_order(0)
{
}

SettingValue::SettingValue(const std::wstring& sValue,const std::wstring& sKey,int order/* = 0*/) : m_value(sValue),m_key(sKey),m_order(order)
{
}

SettingValue::SettingValue(const SettingValue& src) : m_value(src.m_value),m_key(src.m_key),m_order(src.m_order)
{
}

SettingValue& SettingValue::operator=(const SettingValue& right)
{
	m_value = right.m_value;
	m_key = right.m_key;
	m_order = right.m_order;

	return *this;
}
	

const std::wstring& SettingValue::GetValue() const
{
	return m_value;
}

const std::wstring& SettingValue::GetKey() const
{
	return m_key;
}

int SettingValue::GetOrder() const
{
	return m_order;
}
	
void SettingValue::SetValue(const std::wstring& sValue)
{
	m_value = sValue;
}

void SettingValue::SetKey(const std::wstring& sKey)
{
	m_key = sKey;
}

void SettingValue::SetOrder(int order)
{
	m_order = order;
}
	
