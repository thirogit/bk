#if !defined(__ALIGNUTIL_H__)
#define __ALIGNUTIL_H__

#include <string>
#include "../TextAlignment.h"

namespace AlignUtil
{
	std::wstring ToString(HorizontalAlignment alignment);
	HorizontalAlignment FromStringH(const std::wstring& alignment);
	
	std::wstring ToString(VerticalAlignment alignment);
	VerticalAlignment FromStringV(const std::wstring& alignment);
	
};

#endif 
