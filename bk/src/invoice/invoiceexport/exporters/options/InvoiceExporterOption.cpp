#include "stdafx.h"
#include "InvoiceExporterOption.h"

InvoiceExporterOption::InvoiceExporterOption(const InvoiceExporterOption& src)
{
	m_Name = src.m_Name;
	m_Value = src.m_Value;	
}

InvoiceExporterOption::InvoiceExporterOption(const std::wstring& sName) : m_Name(sName)
{
}

const std::wstring& InvoiceExporterOption::name() const
{
	return m_Name;
}


const std::wstring& InvoiceExporterOption::get() const
{
	return m_Value;
}

void InvoiceExporterOption::set(const std::wstring& value)
{	
	m_Value = value;
}
