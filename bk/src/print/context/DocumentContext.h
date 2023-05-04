#ifndef __DOCUMENTCONTEXT_H__
#define __DOCUMENTCONTEXT_H__

#include <data\IInvoice.h>
#include <data\types\CowSex.h>
#include "CurrencySymbol.h"
#include "QtyUnit.h"
#include "..\DocumentTerritory.h"

class DocumentContext
{
public:
	virtual std::wstring GetNumberText(unsigned int number) = 0;
	virtual std::wstring GetPriceText(const Decimal& price, const CurrencySymbol& currencySymbols) = 0;
	virtual CurrencySymbol GetCurrencySymbol() const = 0;
	virtual std::wstring GetQtyUnitText(QtyUnit unit) const = 0;
	virtual std::wstring GetPayWayText(PayWay payway) const = 0;
	virtual std::wstring GetCowSexText(const CowSex& cowsex) const = 0;
	virtual DecimalFormat GetPriceFormat() = 0;
	virtual DecimalFormat GetWeightFormat() = 0;
	virtual DecimalFormat GetPricePerUnitFormat() = 0;
	virtual const DocumentTerritory* GetTeritory() const = 0;
	virtual ~DocumentContext() = 0;
};

#endif