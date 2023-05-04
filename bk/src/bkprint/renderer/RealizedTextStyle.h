#ifndef __REALIZEDTEXTSTYLE_H__
#define __REALIZEDTEXTSTYLE_H__

#include <wingdi.h>

namespace bkprint
{
	class RealizedTextStyle
	{
	public:
		RealizedTextStyle();
		~RealizedTextStyle();

		DWORD GetTextAlignment() const;
		COLORREF GetTextColor() const;
		HFONT GetFont() const;		
		void SetTextAlignment(DWORD);
		void SetTextColor(COLORREF);
		void SetFont(HFONT);
	private:
		DWORD m_txtAlignment;			
		COLORREF m_clrText;
		HFONT m_font;				
	};

	
}
#endif