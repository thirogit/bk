#ifndef __PRICETOTEXT_H__
#define __PRICETOTEXT_H__

#include "../numbertotext/NumberToText.h"
#include <types\Decimal.h>
#include "../CurrencySymbol.h"

class PriceToText 
{
public:
	PriceToText(NumberToText* numberToText);
	std::wstring Translate(const Decimal& price,const CurrencySymbol& currency);
private:
	NumberToText* m_pNumber2Txt;
};

#endif