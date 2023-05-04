#ifndef __STRINGLOADER_H__
#define __STRINGLOADER_H__

#include <string>
#include <cstdint>

class StringLoader
{
public:
	StringLoader(uint32_t uStrResourceId);
	operator std::wstring() const;
	const wchar_t* c_str() const;
	
protected:
	std::wstring m_String;
};

#endif