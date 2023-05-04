#include "stdafx.h"
#include "ComparableFieldDefinition.h"


ComparableFieldDefinition::ComparableFieldDefinition(const std::wstring& optionCode) : m_code(optionCode)
{	
}
	

const std::wstring& ComparableFieldDefinition::GetCode() const
{
	return m_code;
}


ComparableFieldDefinition::ComparableFieldDefinition(const ComparableFieldDefinition& src)
{
	m_code = src.m_code;
	m_name = src.m_name;
}

void ComparableFieldDefinition::SetName(const std::wstring& name)
{
	m_name = name;
}


const std::wstring& ComparableFieldDefinition::GetName() const
{
	return m_name;
}