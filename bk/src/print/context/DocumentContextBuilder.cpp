#include "stdafx.h"
#include "DocumentContextBuilder.h"
#include "text\QtyUnitText.h"
#include "../../text/PayWayString.h"
#include "../../text/CowSexText.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"


DocumentContextBuilder::DocumentContextImpl::DocumentContextImpl()
{
	m_pPriceToTxt = NULL;
	m_pNumToTxt = NULL;
	m_pCurrency = NULL;
}

DocumentContextBuilder::DocumentContextImpl::~DocumentContextImpl()
{
	delete m_pPriceToTxt;
	delete m_pNumToTxt;
	delete m_pCurrency;
}

std::wstring DocumentContextBuilder::DocumentContextImpl::GetNumberText(unsigned int number)
{
	return m_pNumToTxt->Translate(number);
}

std::wstring DocumentContextBuilder::DocumentContextImpl::GetPriceText(const Decimal& price, const CurrencySymbol& currencySymbols)
{
	return m_pPriceToTxt->Translate(price, currencySymbols);
}

CurrencySymbol DocumentContextBuilder::DocumentContextImpl::GetCurrencySymbol() const
{
	return *m_pCurrency;
}

std::wstring DocumentContextBuilder::DocumentContextImpl::GetQtyUnitText(QtyUnit unit) const
{
	return QtyUnitText(unit).ToString();
}

std::wstring DocumentContextBuilder::DocumentContextImpl::GetPayWayText(PayWay payway) const
{
	return PayWayString(payway).ToString();
}

std::wstring DocumentContextBuilder::DocumentContextImpl::GetCowSexText(const CowSex& cowsex) const
{
	return CowSexText(cowsex).ToString();
}

DecimalFormat DocumentContextBuilder::DocumentContextImpl::GetPriceFormat()
{
	return DecimalFormat(2, ',');
}

DecimalFormat DocumentContextBuilder::DocumentContextImpl::GetWeightFormat()
{
	return DecimalFormat(3, ',');
}

DecimalFormat DocumentContextBuilder::DocumentContextImpl::GetPricePerUnitFormat()
{
	return DecimalFormat(5, ',');
}

const DocumentTerritory* DocumentContextBuilder::DocumentContextImpl::GetTeritory() const
{
	return m_territory;
}

//#################################################################################################

DocumentContext* DocumentContextBuilder::Create(DocumentTerritory* territory)
{
	DocumentContextImpl* context = new DocumentContextImpl();

	context->m_pNumToTxt = new NumberToText();
	context->m_pPriceToTxt = new PriceToText(context->m_pNumToTxt);
	context->m_pCurrency = new CurrencySymbol(StringLoader(IDS_CURRENCY_PLN_SYMBOL), StringLoader(IDS_CURRENCY_PLN100_SYMBOL));
	context->m_territory = territory;
	return context;
}