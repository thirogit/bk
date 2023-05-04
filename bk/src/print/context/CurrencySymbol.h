#ifndef __CURRENCYSYMBOL_H__
#define __CURRENCYSYMBOL_H__

#include <string>

class CurrencySymbol
{
public:
	CurrencySymbol(const std::wstring& crrency, const std::wstring& fraction);
	
	const std::wstring& Symbol() const;
	const std::wstring& Fraction() const;

private:
	std::wstring m_currencysymbol;
	std::wstring m_fractionsymbol;
};


#endif