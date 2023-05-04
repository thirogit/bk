#include "stdafx.h"
#include "PrintOption.h"

PrintOption::PrintOption(const PrintOption& src)
{
	m_Name = src.m_Name;
	m_Value = src.m_Value;	
}

PrintOption::PrintOption(const std::wstring& sName) : m_Name(sName)
{
}

const std::wstring& PrintOption::name() const
{
	return m_Name;
}


const std::wstring& PrintOption::get() const
{
	return m_Value;
}

void PrintOption::set(const std::wstring& value)
{	
	m_Value = value;
}
