#include "stdafx.h"
#include "NullString.h"


NullString::NullString() 
{
}

NullString::NullString(const std::wstring& value) : m_Value(value)
{
}

void NullString::CopyFrom(const NullString& copy)
{	
	m_Value = copy.m_Value;
}

NullString::NullString(const NullString& copy)
{
	CopyFrom(copy);
}

NullString& NullString::operator=(const std::wstring& right)
{
	m_Value = right;
	return *this;
}

NullString& NullString::operator=(const NullString& right)
{
	CopyFrom(right);
	return *this;
}

bool NullString::IsNull() const
{
	return !m_Value.is_initialized();
}

void NullString::MakeNull()
{
	m_Value.reset();
}

std::wstring NullString::AsString() const
{
	std::wstring ret;
	if(!IsNull())
	{		
		ret = m_Value.get();		
	}
	return ret;
}
