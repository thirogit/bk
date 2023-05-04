#ifndef __PRINTOPTION_H__
#define __PRINTOPTION_H__

#include <string>

class PrintOption
{
public:
	PrintOption(const PrintOption& src);
	PrintOption(const std::wstring& sName);

	const std::wstring& name() const;	
	const std::wstring& get() const;	

	void set(const std::wstring& str);	
private:
	std::wstring m_Name;
	std::wstring m_Value;
};

#endif