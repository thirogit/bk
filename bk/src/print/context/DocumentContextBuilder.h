#ifndef __DOCUMENTCONTEXTBUILDER_H__
#define __DOCUMENTCONTEXTBUILDER_H__

#include "DocumentContext.h"
#include "pricetotext/PriceToText.h"
#include "CurrencySymbol.h"
#include "QtyUnit.h"
#include "../DocumentTerritory.h"

class DocumentContextBuilder 
{
	class DocumentContextImpl : public DocumentContext
	{
	public:
		DocumentContextImpl();
		virtual ~DocumentContextImpl();
		virtual std::wstring GetNumberText(unsigned int number);
		virtual std::wstring GetPriceText(const Decimal& price, const CurrencySymbol& currencySymbols);
		virtual CurrencySymbol GetCurrencySymbol() const;
		virtual std::wstring GetQtyUnitText(QtyUnit unit) const;
		virtual std::wstring GetPayWayText(PayWay payway) const;
		virtual std::wstring GetCowSexText(const CowSex& cowsex) const;
		virtual DecimalFormat GetPriceFormat();
		virtual DecimalFormat GetWeightFormat();
		virtual DecimalFormat GetPricePerUnitFormat();
		virtual const DocumentTerritory* GetTeritory() const;
	private:
		PriceToText* m_pPriceToTxt;
		NumberToText* m_pNumToTxt;
		CurrencySymbol* m_pCurrency;
		DocumentTerritory* m_territory;
		
		friend class DocumentContextBuilder;
	};

public:
	DocumentContext* Create(DocumentTerritory* territory);
};



#endif
