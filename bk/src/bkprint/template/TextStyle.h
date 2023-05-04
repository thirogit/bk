#ifndef __TEXTSTYLE_H__
#define __TEXTSTYLE_H__

#include "TextAlignment.h"
#include "../../uitypes/Color.h"
#include "../types/Font.h"

namespace bkprint
{

	class TextStyle : public Nullable
	{
		struct _TextStyle
		{
			Color m_txtclr;
			HorizontalAlignment m_halign;
			VerticalAlignment m_valign;
			Font m_font;
		};		
		
		void EnsureValue();

	public:
		TextStyle();
		TextStyle(const TextStyle &copy);		
		TextStyle& operator=(const TextStyle &right);
				
		Font GetFont() const;
		Color GetTextColor() const;
		VerticalAlignment GetVerAlign() const;
		HorizontalAlignment GetHorAlign() const;

		void SetFont(const Font& font);
		void SetTextColor(const Color& color);
		void SetVerAlign(VerticalAlignment align);
		void SetHorAlign(HorizontalAlignment align);


		virtual void MakeNull();
		virtual bool IsNull() const;

	

	protected:
		boost::optional<_TextStyle> m_Value;

	private:
		void _String() const;
#ifdef _DEBUG
		mutable std::wstring m_string;
#endif

	};

}
#endif