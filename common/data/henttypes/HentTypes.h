#ifndef __HENTTYPES_H__
#define __HENTTYPES_H__

#include <string>
#include "../HentType.h"

namespace HentTypes
{	
	HentType FromChar(wchar_t cHentType);
	std::wstring ToString(HentType henttype);
	HentType FromString(const std::wstring& type);
};

#endif