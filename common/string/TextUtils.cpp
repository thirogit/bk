#include "stdafx.h"
#include <stdlib.h>
#include "TextUtils.h"
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>

#ifdef USE_UTF8CPP
#include <utf8.h>
#else
#include <codecvt>
#endif

namespace TextUtils
{

	std::wstring PadBegin(const std::wstring& s, wchar_t padChar, int requiredLength)
	{
		int missingLength = requiredLength - s.length();
		if (missingLength > 0)
		{
			return std::wstring(missingLength, padChar) + s;
		}
		return s;
	}

	void throwInvalidNumber(const std::wstring& str)
	{
		BOOST_THROW_EXCEPTION(AlphaToFloatException(Format(L"'%s' is not a valid number", str)));
	}

	double AlphaToFloat(const std::wstring& str,wchar_t decimalPt)
	{
		const wchar_t* s = str.c_str();
		double a = 0.0;

		int c;
		int isneg = 0;
		int e = 0;
		while((c = *s++) == ' ')
				;
		if (c == '-')
			isneg = 1;
		else if (c=='+')
			;
		else
			--s;

		while ((c = *s++) != '\0' && c != decimalPt) {

			if (isdigit(c))
			{
				a = a*10.0 + (c - '0');
			}
			else
			{
				throwInvalidNumber(str);
			}
		}

		if (c == decimalPt) 
		{
			while ((c = *s++) != '\0') {

				if (isdigit(c))
				{
					a = a*10.0 + (c - '0');					
					e = e - 1;
				}
				else
				{
					throwInvalidNumber(str);
				}
			}
		}

		while (e < 0) {
			a *= 0.1;
			e++;
		}

		if (isneg)
			a = -a;
		return a;
	}

#if 0
	CString ByteSizeToString(ULONGLONG size)
	{
	
		const char units[] = "BKMGTPE";
		const ULONGLONG scale = 1024;
		const char *up = units;
		ULONGLONG osize = 0;
		CString outStr;

		/*
		* Convert the input size to a round number of the appropriately
		* scaled units (saved in 'size') and a remainder (saved in 'osize').
		*/
		while (size >= scale && up < (units + sizeof (units) - 2)) {
			up++;
			osize = size;
			size = (size + (scale / 2)) / scale;
		}
		/*
		* Format the result using at most one decimal place and the unit
		* depending upon the amount of remainder (same as df -h algorithm).
		*/
		if (osize != 0 && (osize / scale) < 10)
			outStr.Format("%.1f%c", (float)osize / scale, *up);
		else if (size != 0)
			outStr.Format("%u%c", (UINT)size, *up);
		else
			outStr = "0";

		return outStr;
	}


#endif


	bool IsNumber(const std::wstring& str)
	{
		for(int i = 0;i < str.size();i++)
		{
			if(!iswdigit(str[i]))
				return false;
		}
		return true;
	}

	bool IsAlpha(const std::string& str)
	{
		for(int i = 0;i < str.size();i++)
		{
			if(!isalpha(str[i]))
				return false;
		}
		return true;
	}

	std::wstring Right(const std::wstring& s,int chars)
	{
				
		if (chars < 0)
			chars = 0;

		int len = s.length();
		if( chars >= len )
		{
			return s;
		}

		return s.substr(len-chars, chars);

	}

	std::wstring Left(const std::wstring& s,int chars)
	{
		return s.substr(0,chars);
	}

	std::wstring FormatV(const wchar_t* szFmt,va_list args)
	{
		std::wstring sResult;		
		//int size = _vscprintf( szFmt, args )+1;
		
		int size = vsnwprint_safe(NULL, 0, szFmt, args) + 1;
		wchar_t* szBuffer = new wchar_t[size];
		vsnwprint_safe(szBuffer, size, szFmt, args);
		sResult = szBuffer;
		delete[] szBuffer;
		return sResult;
	}

	std::wstring FormatA(const wchar_t* szFmt,...)
	{
		va_list vl;
		va_start(vl,szFmt);
		std::wstring s = FormatV(szFmt,vl);	
		va_end(vl);
		return s;	
	}

	std::wstring Quote(const std::wstring& s)
	{
		std::wstringbuf buffer;
		std::wostream os (&buffer);
		os << L'\'';
		std::for_each(s.begin(),s.end(),[&os](wchar_t ch)
						{
							if(ch == L'\'' || ch == L'\\')
							os << L'\\';
							os << ch;										
						});
		os << L'\'';
		os.flush();
		return buffer.str();

	}

	std::wstring JoinLines(const std::wstring& s, wchar_t joinChar)
	{
		std::wstringbuf buffer;
		std::wostream os(&buffer);
		
		std::for_each(s.begin(), s.end(), [&os, joinChar](char ch)
		{
			if (ch == L'\r')
			{
				return;
			}
			
			if (ch == L'\n')
			{
				os << joinChar;
				return;
			}

			os << ch;
		});
		
		os.flush();
		return buffer.str();
	}
		
		

	std::wstring Remove(const std::wstring& s,wchar_t c)
	{
		std::wstring result;

		for(const auto ch : s)
		{
			if(ch != c)
				result += ch;
		}

		return result;
	}

	bool IsAllWhitespace(const std::wstring& s)
	{
		auto it = s.begin();
		while (it != s.end())
		if (!iswspace(*it++))
			return false;

		return true;
			
	}

	std::wstring RemoveWhitespace(const std::wstring& s)
	{
		std::wstring result;
		auto it = s.begin();
		while (it != s.end()) if (!iswspace(*it)) result += *it;

		return result;
	}


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

	bool startsWith(const std::wstring& s, const std::wstring& prefix)
	{
		const std::wstring& ss = Left(s, prefix.length());
		return ss == prefix;
	}


	bool ContainsNoCase(const std::wstring& s, const std::wstring& part)
	{
		return boost::algorithm::icontains(s, part);
	}


	std::wstring IfEmpty(const std::wstring& s, const std::wstring& ifEmpty)
	{
		if (s.empty())
		{
			return ifEmpty;
		}

		return s;
	}

	bool IsOneOf(char v, char c1, char c2, char c3,char c4)
	{
		return v == c1 || v == c2 || v == c3 || v == c4;
	}

	bool IsOneOf(char v, char c1, char c2, char c3)
	{
		return v == c1 || v == c2 || v == c3;
	}

	
}

std::wstring w(const std::string& s)
{
	return TextUtils::FromUTF8(s);
}

std::string u(const std::wstring& s)
{
	return TextUtils::ToUTF8(s);
}

