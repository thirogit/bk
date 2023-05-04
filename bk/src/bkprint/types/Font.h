#ifndef __FONT_H__
#define __FONT_H__

#include <string>

namespace bkprint
{
	class Font
	{
	public:
		Font();
		Font(const std::wstring& faceName,int height);
		
		void SetFaceName(const std::wstring faceName);
		void SetHeight(int height);		
		void SetIsBold(bool bold);
		void SetIsItalic(bool italic);
		void SetIsUnderline(bool underline);
		void SetIsStrikeout(bool strikeout);
		
		const std::wstring& GetFaceName() const;
		int GetHeight() const;
		bool GetIsBold() const;
		bool GetIsItalic() const;
		bool GetIsUnderline() const;
		bool GetIsStrikeout() const;

		std::wstring ToString() const;


	private:
		std::wstring m_faceName;
		int m_height;
		bool m_bold;
		bool m_underline;
		bool m_italic;
		bool m_strikeout;
	};

}
#endif