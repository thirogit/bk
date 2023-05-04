#ifndef __TEXTOBJECT_H__
#define __TEXTOBJECT_H__

#include "RectObject.h"
#include <string>



namespace bkprint
{
	class TemplateRenderer;

	class TextObject : public RectObject
	{
	public:
		TextObject();
		virtual void Draw(CDC* pDC);
	protected:
		std::wstring m_text;
		HFONT m_font;
		COLORREF m_textColor;
		DWORD m_textAlignment;
		CRect m_padding;
		friend class TemplateRenderer;
	};

}


#endif