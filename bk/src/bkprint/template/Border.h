#ifndef __BORDER_H__
#define __BORDER_H__

#include "../../uitypes/Color.h"

namespace bkprint
{
	class Border : public Nullable
	{		
		struct _Border
		{
			Color m_lineColor;
			int m_lineWidth;
		};

	public:
		Border();
		Border(const Border& src);
		Border(int width, const Color& color);
		Border& operator=(const Border& right);

		int GetWidth() const;
		Color GetColor() const;
		
		void SetWidth(int width);
		void SetColor(const Color& color);

		virtual void MakeNull();
		virtual bool IsNull() const;

	private:
		boost::optional<_Border> m_Value;

	private:
		void _String() const;
#ifdef _DEBUG
		mutable std::wstring m_string;
#endif
		
	};

	bool operator<(const Border&, const Border&);
	bool operator>(const Border&, const Border&);
}



#endif
