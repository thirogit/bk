#include "stdafx.h"
#include "Utf8.h"

#ifdef USE_UTF8CPP
#include <utf8.h>
#else
#include <codecvt>
#endif

namespace Utf8
{

	std::string ToUTF8(const std::wstring& s)
	{
#ifndef USE_UTF8CPP
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf16conv;
		return utf16conv.to_bytes(s);
		
#else
		std::string result;
		utf8::utf16to8(s.begin(),s.end(),std::back_inserter(result));
		return result;
#endif
	}

	std::wstring FromUTF8(const std::string& s)
	{

#ifndef USE_UTF8CPP
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf16conv;
		return utf16conv.from_bytes(s);

#else
		std::wstring result;
		utf8::utf8to16(s.begin(), s.end(), std::back_inserter(result));
		return result;
#endif
		
	}

}

