#include "stdafx.h"
#include "IntString.h"
#include "string/TextUtils.h"

IntString::IntString(int i) 
{
	m_String = TextUtils::FormatA(L"%d",i);
}

IntString::IntString(unsigned int i)
{
	m_String = (boost::wformat(L"%u") % i).str();
}

#ifndef SIZET_IS_UINT 
IntString::IntString(size_t size)
{
	m_String = (boost::wformat(L"%u") % size).str();
}
#endif

IntString::IntString(long long u)
{
	m_String = (boost::wformat(L"%d") % u).str();
}

IntString::operator std::wstring()  const
{
	return m_String;
}

const wchar_t* IntString::c_str() const
{
	return m_String.c_str();
}

std::wstring operator+(const IntString& left,const std::wstring& right)
{
	return (std::wstring)left + right;
}

std::wstring operator+(const std::wstring& left,const IntString& right)
{
	return left + (std::wstring)right;
}


