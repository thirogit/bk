#include "stdafx.h"
#include "DocumentOptionDefinition.h"
#include <string/IntString.h>

DocumentOptionDefinition::DocumentOptionDefinition(const std::wstring& optionCode, DocumentOptionType type) : m_code(optionCode), m_type(type)
{	
}
	

const std::wstring& DocumentOptionDefinition::GetCode() const
{
	return m_code;
}

DocumentOptionType DocumentOptionDefinition::GetType() const
{
	return m_type;
}

DocumentOptionDefinition::DocumentOptionDefinition(const DocumentOptionDefinition& src)
{
	m_type = src.m_type;
	m_code = src.m_code;
	m_name = src.m_name;
	m_defaultValue = src.m_defaultValue;
}

void DocumentOptionDefinition::SetName(const std::wstring& name)
{
	m_name = name;
}

void DocumentOptionDefinition::SetDefualtValue(const std::wstring& value)
{
	m_defaultValue = value;
}

const std::wstring& DocumentOptionDefinition::GetDefaultValue() const
{
	return m_defaultValue;
}

const std::wstring& DocumentOptionDefinition::GetName() const
{
	return m_name;
}