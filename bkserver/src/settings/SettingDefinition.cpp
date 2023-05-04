#include "stdafx.h"
#include "SettingDefinition.h"


SettingDefinition::SettingDefinition(const std::wstring& sNamePattern) : m_namePattern(sNamePattern)
{
}

SettingDefinition::SettingDefinition(const SettingDefinition& src)
{
	m_type = src.m_type;
	m_namePattern = src.m_namePattern;
}

const std::wstring& SettingDefinition::GetNamePattern() const
{
	return m_namePattern;
}
	
void SettingDefinition::SetType(const SettingType& type)
{
	m_type = type;
}

const SettingType& SettingDefinition::GetType() const
{
	return m_type;
}
	
int SettingDefinition::GetOrder() const
{
	return m_order;
}

void SettingDefinition::SetOrder(int order)
{
	m_order = order;
}