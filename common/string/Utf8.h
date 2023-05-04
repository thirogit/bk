#ifndef __UTF8_H__
#define __UTF8_H__

#include <string>

namespace Utf8
{
	std::string ToUTF8(const std::wstring& s);
	std::wstring FromUTF8(const std::string& s);	
}

#endif
