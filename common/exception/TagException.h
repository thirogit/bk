#ifndef __TAGEXCEPTION_H__
#define __TAGEXCEPTION_H__

#include <string>
#include <stdexcept>
#include "../string/Utf8.h"

class wexception : public std::runtime_error
{
public:
	wexception(const std::wstring& sWhat) : std::runtime_error(Utf8::ToUTF8(sWhat))
	{
		m_message = sWhat;
	}
	
	const std::wstring& message()
	{
		return m_message;
	}

	const wchar_t* wwhat()
	{
		return m_message.c_str();
	}

private:
	std::wstring m_message;
};

template<typename TAG>
class TagException : public wexception
{
public:
	TagException(const std::wstring& sWhat) : wexception(sWhat)
	{		
	}	
};

#endif
