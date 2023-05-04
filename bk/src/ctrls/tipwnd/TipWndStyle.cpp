#include "stdafx.h"
#include "TipWndStyle.h"
#include "../../uitypes/Color.h"

TipWndStyle::TipWndStyle() 
{
}

TipWndStyle& TipWndStyle::inner(const Color& color)
{
	m_BorderInner = color;
	return *this;
}

TipWndStyle& TipWndStyle::outer(const Color& color)
{
	m_BorderOuter = color;
	return *this;
}

TipWndStyle& TipWndStyle::text(const Color& color)
{
	m_TextColour = color;
	return *this;
}

TipWndStyle& TipWndStyle::margin(const NullInt& imargin)
{
	m_margin = imargin;
	return *this;
}
	
const Color& TipWndStyle::inner() const
{
	return m_BorderInner;
}

const Color& TipWndStyle::outer() const
{
	return m_BorderOuter;
}

const Color& TipWndStyle::text() const
{
	return m_TextColour;
}

const NullInt& TipWndStyle::margin() const
{
	return m_margin;
}

const NullInt& TipWndStyle::textsize() const
{
	return m_height;
}

TipWndStyle& TipWndStyle::textsize(const NullInt& height)
{
	m_height = height;
	return *this;
}