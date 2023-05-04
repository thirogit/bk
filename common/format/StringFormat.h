#ifndef __STRINGFORMAT_H__
#define __STRINGFORMAT_H__

#include <string>

class StringFormat
{
public:
	StringFormat(const wchar_t* szFormat,...);
	operator std::wstring() const;
	const wchar_t* c_str() const;
	
protected:
	std::wstring m_String;
};

#endif