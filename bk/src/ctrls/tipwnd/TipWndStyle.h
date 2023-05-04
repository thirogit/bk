#ifndef __TIPWNDSTYLE_H__
#define __TIPWNDSTYLE_H__

#include "../../uitypes/Color.h"
#include <types/NullInt.h>

class TipWndStyle
{
public:
	TipWndStyle();	
	
	TipWndStyle& inner(const Color& color);
	TipWndStyle& outer(const Color& color);
	TipWndStyle& text(const Color& color);
	TipWndStyle& margin(const NullInt& imargin);
	TipWndStyle& textsize(const NullInt& height);

	const Color& inner() const;
	const Color& outer() const;
	const Color& text() const;
	const NullInt& margin() const;
	const NullInt& textsize() const;
		
protected:
	
	Color m_TextColour;
	Color m_BorderOuter;
	Color m_BorderInner;
	NullInt m_margin;
	NullInt m_height;
};
#endif