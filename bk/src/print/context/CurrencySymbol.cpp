#include "stdafx.h"
#include "CurrencySymbol.h"

CurrencySymbol::CurrencySymbol(const std::wstring& crrency, const std::wstring& fraction) :
	m_currencysymbol(crrency),m_fractionsymbol(fraction)
{

}
	
const std::wstring& CurrencySymbol::Symbol() const
{
	return m_currencysymbol;
}

const std::wstring& CurrencySymbol::Fraction() const
{
	return m_fractionsymbol;
}
