#include "stdafx.h"
#include "PassportFieldDescriptor.h"

PassportFieldDescriptor::PassportFieldDescriptor(const std::wstring& sFieldName,bool bIsBarcode)
{
	m_sFieldName = sFieldName;
	m_bIsBarcode = bIsBarcode;
}

bool PassportFieldDescriptor::IsBarcode() const
{
	return m_bIsBarcode;
}

const std::wstring& PassportFieldDescriptor::GetFieldName() const
{
	return m_sFieldName;
}
