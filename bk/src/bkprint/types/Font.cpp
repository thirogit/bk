#include "stdafx.h"
#include "Font.h"
#include <string/TextUtils.h>

namespace bkprint
{	
	Font::Font()
	{
		m_height = 0;
		m_bold = false;
		m_underline = false;
		m_italic = false;
		m_strikeout = false;
	}
	
	Font::Font(const std::wstring& faceName,int height) : Font()
	{
		m_height = height;
		m_faceName = faceName;
	}
	
	void Font::SetFaceName(const std::wstring m_strikeout)
	{
		m_faceName = m_strikeout;
	}
	
	void Font::SetHeight(int height)
	{
		m_height = height;
	}
	
	void Font::SetIsBold(bool bold)
	{
		m_bold = bold;
	}
	
	void Font::SetIsItalic(bool italic)
	{
		m_italic = italic;
	}
	
	void Font::SetIsUnderline(bool underline)
	{
		m_underline = underline;
	}
	
	void Font::SetIsStrikeout(bool strikeout)
	{
		m_strikeout = strikeout;
	}

	const std::wstring& Font::GetFaceName() const
	{
		return m_faceName;
	}

	int Font::GetHeight() const
	{
		return m_height;
	}

	bool Font::GetIsBold() const
	{
		return m_bold;
	}

	bool Font::GetIsItalic() const
	{
		return m_italic;
	}

	bool Font::GetIsUnderline() const
	{
		return m_underline;
	}

	bool Font::GetIsStrikeout() const
	{
		return m_strikeout;
	}

	std::wstring Font::ToString() const
	{
		std::wstring s = m_faceName + L" " + TextUtils::Format(L"%d", m_height);
		if (m_bold) s += L"B";
		if (m_italic) s += L"I";
		if (m_underline) s += L"U";
		if (m_strikeout) s += L"S";
		return s;
	}

}
