#include "stdafx.h"
#include "Color.h"
#include <string/TextUtils.h>


bool operator<(const Color& right, const Color& left)
{
	return (right > left);
}

bool operator>(const Color& right, const Color& left)
{
	if (left.IsNull())
	{
		return false;
	}
	else
	if (right.IsNull())
	{
		return true;
	}

	return left.operator DWORD() > right.operator DWORD();
}

Color::Color()
{
}

Color::Color(uint8_t r,uint8_t g,uint8_t b)
{
	Set(r, g, b);
	_String();
}

Color::Color(const Color& src)
{
	m_Value = src.m_Value;
	_String();
}

Color::Color(COLORREF color)
{	
	operator=(color);
	_String();
}

uint8_t Color::R() const
{
	return (IsNull() ? 0 : m_Value.get().R);
}	

uint8_t Color::G() const
{
	return (IsNull() ? 0 : m_Value.get().G);
}

uint8_t Color::B() const
{
	return (IsNull() ? 0: m_Value.get().B);
}

void Color::MakeNull()
{
	m_Value.reset();
	_String();
}

bool Color::IsNull() const
{
	return !m_Value.is_initialized();
}

Color::operator COLORREF() const
{
	if(!IsNull())
		return RGB(R(),G(),B());

	return RGB(0,0,0);
}

void Color::Set(uint8_t r, uint8_t g, uint8_t b)
{
	_Color clr;
	clr.R = r;
	clr.G = g;
	clr.B = b;
	m_Value = clr;
}


Color& Color::operator=(COLORREF color)
{
	_Color clr;
	clr.R = GetRValue(color);
	clr.G = GetGValue(color);
	clr.B = GetBValue(color);
	m_Value = clr;
	return *this;
}

void Color::_String() const
{
#ifdef _DEBUG
	if (IsNull())
	{
		m_string.clear();
	}
	else
	{
		m_string = TextUtils::Format(L"#%02X%02X%02X", m_Value->R, m_Value->G, m_Value->G);
	}
#endif
}
