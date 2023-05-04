#include "stdafx.h"
#include "TextObject.h"

namespace bkprint
{

	TextObject::TextObject() : m_font(NULL), m_textColor(RGB(0, 0, 0)), m_textAlignment(0)
	{
	}

	void TextObject::Draw(CDC* pDC)
	{
		RectObject::Draw(pDC);
		
		CRect calcrc(m_Position);
		CRect rc(&calcrc);

		COLORREF oldColor = pDC->GetTextColor();
		int oldMode = pDC->GetBkMode();


		pDC->SetTextColor(m_textColor);		
		pDC->SetBkMode(TRANSPARENT);
		HFONT oldFont = (HFONT)::SelectObject(pDC->GetSafeHdc(), m_font);
				
		int height = abs(pDC->DrawText(m_text.c_str(), &calcrc, DT_WORDBREAK | DT_CALCRECT)),
			rcheight = abs((rc.bottom - rc.top));
				

		if (height < rcheight)
		{
			if (m_textAlignment & DT_VCENTER){
				rc.top += (rcheight - height) / 2; 
			}
			else if (m_textAlignment & DT_BOTTOM) {
				rc.top += (rcheight - height);
			}			
		}

		pDC->DrawText(m_text.c_str(), rc, DT_WORDBREAK | (m_textAlignment & (~(DT_VCENTER | DT_BOTTOM))));

		::SelectObject(pDC->GetSafeHdc(), oldFont);
		pDC->SetBkMode(oldMode);
		pDC->SetTextColor(oldColor);
		
	}
	

}
