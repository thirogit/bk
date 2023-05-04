#ifndef __INTSTRING_H__
#define __INTSTRING_H__

#include <stdint.h>
#include "../format/StringFormat.h"
#include <type_traits>

#if SIZE_MAX == UINT32_MAX
#define SIZET_IS_UINT
#endif

class IntString 
{
public:
	IntString(int i);
	IntString(unsigned int i);

#ifndef SIZET_IS_UINT 
	IntString(size_t u);
#endif

	IntString(long long u);
	operator std::wstring() const;
	const wchar_t* c_str() const;
private:
	std::wstring m_String;
};

std::wstring operator+(const IntString& left,const std::wstring& right);
std::wstring operator+(const std::wstring& left,const IntString& right);

#endif