#include "stdafx.h"
#include "StringLoader.h"
#include <afxstr.h>

StringLoader::StringLoader(uint32_t uStrResourceId)
{
	CString s;
	s.LoadString(uStrResourceId);
	m_String = s.operator LPCWSTR();
}

StringLoader::operator std::wstring() const
{
	return m_String;
}

const wchar_t* StringLoader::c_str() const
{
	return m_String.c_str();
}