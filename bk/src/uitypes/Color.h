#pragma once

#include <types\Nullable.h>
#include <boost\optional.hpp>
#include <wingdi.h>

typedef unsigned char uint8_t;

class Color : public Nullable
{
	struct _Color
	{		
		uint8_t R;
		uint8_t G;
		uint8_t B;	
	};
	
public:
	Color();
	Color(const Color& src);
	Color(uint8_t r,uint8_t g,uint8_t b);
	explicit Color(COLORREF color);

	uint8_t R() const;
	uint8_t G() const;
	uint8_t B() const;

	void Set(uint8_t r, uint8_t g, uint8_t b);

	operator COLORREF() const;
	Color& operator=(COLORREF color);

	virtual void MakeNull();
	virtual bool IsNull() const;
	
private:
	boost::optional<_Color> m_Value;
	void _String() const;
#ifdef _DEBUG
	mutable std::wstring m_string;
#endif
};

bool operator<(const Color&, const Color&);
bool operator>(const Color&, const Color&);