#pragma once

#include <types\Nullable.h>
#include <boost\optional.hpp>
#include <wingdi.h>

typedef unsigned char uint8_t;

class Image : public Nullable
{
public:
	Image();
	Image(const Image& src);
	static Image Load(uint32_t resource);
	void Draw(HDC* pDC,const CPoint& point);
	
	virtual void MakeNull();
	virtual bool IsNull() const;
	
private:
	
};

