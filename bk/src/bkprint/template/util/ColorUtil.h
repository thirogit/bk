#if !defined(__COLORUTIL_H__)
#define __COLORUTIL_H__

#include <string>
#include <exception/TagException.h>
#include "../../../uitypes/Color.h"

typedef TagException<struct tag_InvalidColorException> InvalidColorException;

namespace ColorUtil
{
	std::wstring ToString(const Color& color);
	Color FromString(const std::wstring& color);	
};

#endif 
