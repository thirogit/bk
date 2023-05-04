#include "stdafx.h"
#include "Border.h"
#include <string/TextUtils.h>
#include "util/ColorUtil.h"

namespace bkprint
{

	bool operator>(const Border& left, const Border& right)
	{
		if (left.IsNull())
		{
			return false;
		}
		else
		if (right.IsNull()) return true;

		int leftWidth = left.GetWidth(), rightWidth = right.GetWidth();
		if (leftWidth == rightWidth)
		{
			return left.GetColor() > right.GetColor();
		}

		return leftWidth > rightWidth;
	}

	bool operator<(const Border& left, const Border& right)
	{
		return (right > left);
	}


	Border::Border() 
	{
		_String();
	}

	Border::Border(const Border& src)
	{
		operator=(src);
	}

	Border::Border(int width, const Color& color)
	{
		_Border border;
		border.m_lineColor = color;
		border.m_lineWidth = width;
		m_Value = border;
		_String();
		
	}

	Border& Border::operator=(const Border& right)
	{
		m_Value = right.m_Value;
		_String();
		return *this;
	}

	int Border::GetWidth() const
	{
		if (IsNull()) return -1;

		return m_Value->m_lineWidth;
	}

	Color Border::GetColor() const
	{
		if (IsNull()) return Color();

		return m_Value->m_lineColor;
	}
	
	void Border::SetWidth(int width)
	{
		_Border border;
		border.m_lineColor = GetColor();
		border.m_lineWidth = width;
		m_Value = border;
		_String();
	}

	void Border::SetColor(const Color& color)
	{
		_Border border;
		border.m_lineColor = color;
		border.m_lineWidth = GetWidth();
		m_Value = border;
		_String();
	}

	void Border::MakeNull()
	{
		m_Value.reset();
		_String();
	}

	bool Border::IsNull() const
	{
		return !m_Value.is_initialized();
	}

	void Border::_String() const
	{
#ifdef _DEBUG
		if (IsNull())
		{
			m_string.clear();
		}
		else
		{
			m_string = L"width: " + TextUtils::Format(L"%d", m_Value->m_lineWidth) + L", color: " + ColorUtil::ToString(m_Value->m_lineColor);
		}
#endif
	}
	
}