#include "stdafx.h"
#include "SettingPart.h"

SettingPart::SettingPart(const std::wstring& sName) : m_name(sName)
{
}

SettingPart::SettingPart(const std::wstring& sName,const SettingValue& value) : m_name(sName),m_value(value)
{
}

SettingPart::SettingPart(const SettingPart& src) : m_name(src.m_name),m_value(src.m_value)
{
}
	
const std::wstring& SettingPart::GetName() const
{
	return m_name;
}
	
const SettingValue& SettingPart::GetValue() const
{
	return m_value;
}


void SettingPart::SetValue(const SettingValue& value)
{
	m_value = value;
}
