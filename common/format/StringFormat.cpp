#include "stdafx.h"
#include "StringFormat.h"
#include "../string/TextUtils.h"

StringFormat::StringFormat(const wchar_t* szFormat,...)
{	
  va_list vl;
  va_start(vl,szFormat);  
  m_String = TextUtils::FormatV(szFormat, vl);	
}

StringFormat::operator std::wstring()  const
{
	return m_String;
}

const wchar_t* StringFormat::c_str() const
{
	return m_String.c_str();
}

