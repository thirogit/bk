#include "stdafx.h"
#include "TextStyle.h"
#include "util/AlignUtil.h"
#include "util/ColorUtil.h"

namespace bkprint
{

	TextStyle::TextStyle()
	{
		_String();
	};

	void TextStyle::EnsureValue()
	{
		if (!m_Value.is_initialized())
		{
			m_Value = _TextStyle();
			m_Value->m_halign = HorizontalAlignment_Center;
			m_Value->m_txtclr = Color(0, 0, 0);
			m_Value->m_valign = VerticalAlignment_Center;
			m_Value->m_font = Font(L"Arial", 8);
		}		
	}

	void TextStyle::MakeNull()
	{
		m_Value.reset();
		_String();
	}

	bool TextStyle::IsNull() const
	{
		return !m_Value.is_initialized();
	}

	TextStyle::TextStyle(const TextStyle &copy)
	{
		operator=(copy);
	};
	
	TextStyle& TextStyle::operator=(const TextStyle &right)
	{
		m_Value = right.m_Value;
		_String();
		return *this;
	}

	Font TextStyle::GetFont() const
	{
		if (!IsNull())
			return m_Value->m_font;

		return Font();
	}

	
	Color TextStyle::GetTextColor() const
	{
		if (!IsNull())
			return m_Value->m_txtclr;

		return Color();
	}

	VerticalAlignment TextStyle::GetVerAlign() const
	{
		if (!IsNull())
			return m_Value->m_valign;

		return VerticalAlignment_None;
	}

	HorizontalAlignment TextStyle::GetHorAlign() const
	{
		if (!IsNull())
			return m_Value->m_halign;

		return HorizontalAlignment_None;
	}

	void TextStyle::SetFont(const Font& font)
	{
		EnsureValue();
		m_Value->m_font = font;
		_String();
	}

	void TextStyle::SetTextColor(const Color& color)
	{
		EnsureValue();
		m_Value->m_txtclr = color;
		_String();
	}

	void TextStyle::SetVerAlign(VerticalAlignment align)
	{
		EnsureValue();
		m_Value->m_valign = align;
		_String();
	}

	void TextStyle::SetHorAlign(HorizontalAlignment align)
	{
		EnsureValue();
		m_Value->m_halign = align;
		_String();
	}

	void TextStyle::_String() const
	{
#ifdef _DEBUG
		if (IsNull())
		{
			m_string.clear();
		}
		else
		{
			m_string = L"font: " + m_Value->m_font.ToString() + L", valign: " + AlignUtil::ToString(m_Value->m_valign) + L", halign: " + AlignUtil::ToString(m_Value->m_halign) + L", color: " + ColorUtil::ToString(m_Value->m_txtclr);
		}
#endif
	}
}