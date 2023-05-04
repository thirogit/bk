#include "stdafx.h"
#include "PriceToText.h"


PriceToText::PriceToText(NumberToText* numberToText) : m_pNumber2Txt(numberToText)
{

}

std::wstring PriceToText::Translate(const Decimal& price, const CurrencySymbol& currency)
{
	long long whole = price.Integer();
	Decimal mills = price.Fraction()*Decimal(1000L);

	long long cents = mills.Integer();

	cents += ((cents % 10) >= 5 ? 1 : 0);

	return m_pNumber2Txt->Translate(whole) + L' ' + currency.Symbol() + L' ' + m_pNumber2Txt->Translate(cents) + L' ' + currency.Fraction();	
}
