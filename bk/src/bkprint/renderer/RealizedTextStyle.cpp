#include "stdafx.h"
#include "RealizedTextStyle.h"


namespace bkprint
{

	RealizedTextStyle::RealizedTextStyle() : m_txtAlignment(0), m_clrText(RGB(0, 0, 0)), m_font(NULL)
	{
	}

	RealizedTextStyle::~RealizedTextStyle()
	{
		::DeleteObject(m_font);
	}

	DWORD RealizedTextStyle::GetTextAlignment() const
	{
		return m_txtAlignment;
	}

	COLORREF RealizedTextStyle::GetTextColor() const
	{
		return m_clrText;
	}

	HFONT RealizedTextStyle::GetFont() const
	{
		return m_font;
	}

	void RealizedTextStyle::SetTextAlignment(DWORD txtAlignment)
	{
		m_txtAlignment = txtAlignment;
	}

	void RealizedTextStyle::SetTextColor(COLORREF clrText)
	{
		m_clrText = clrText;
	}

	void RealizedTextStyle::SetFont(HFONT font)
	{
		m_font = font;
	}
	
}
