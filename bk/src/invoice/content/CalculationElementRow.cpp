#include "stdafx.h"
#include "CalculationElementRow.h"
#include "../../uiconstants/PrecisionContstants.h"

CalculationElementRow::CalculationElementRow(CalculationElementView* pView) : m_pContent(pView)
{
}

CalculationElementRow::~CalculationElementRow()
{
	delete m_pContent;
}

std::wstring CalculationElementRow::CellValue(uint32_t colId)
{
	switch(colId)
	{
	case CalculationElementViewMember::CalculationElementView_CowNo:
		return m_pContent->GetCowNo().ToString();
	case CalculationElementViewMember::CalculationElementView_Class:
		return m_pContent->GetClass().Object();
	case CalculationElementViewMember::CalculationElementView_Weight:
		return m_pContent->GetWeight().Object().ToString(WEIGHT_PREC);
	case CalculationElementViewMember::CalculationElementView_OtherHent:
		return m_pContent->GetOtherHentAlias();
	case CalculationElementViewMember::CalculationElementView_OtherPrice:
		return m_pContent->GetOtherPrice().Object().ToString(PRICE_PREC);
	//case CalculationElementViewMember::CalculationElementView_Rule:
	case CalculationElementViewMember::CalculationElementView_NetPricePerKg:
		return m_pContent->GetNetPricePerKg().Object().ToString(PRICEPERKG_PREC);
	case CalculationElementViewMember::CalculationElementView_NetPrice:
		return m_pContent->GetNetPrice().Object().ToString(PRICE_PREC);
	case CalculationElementViewMember::CalculationElementView_TaxValue:
		return m_pContent->GetTaxValue().Object().ToString(PRICE_PREC);
	case CalculationElementViewMember::CalculationElementView_GrossPrice:
		return m_pContent->GetGrossPrice().Object().ToString(PRICE_PREC);
	}

	return std::wstring();
}

Color CalculationElementRow::CellColor(uint32_t colId)
{
	switch(colId)
	{
	case CalculationElementViewMember::CalculationElementView_Class:
		return m_pContent->GetClass().GetColor();
	case CalculationElementViewMember::CalculationElementView_Weight:
		return m_pContent->GetWeight().GetColor();
	case CalculationElementViewMember::CalculationElementView_OtherPrice:
		return m_pContent->GetOtherPrice().GetColor();
	case CalculationElementViewMember::CalculationElementView_NetPricePerKg:
		return m_pContent->GetNetPricePerKg().GetColor();
	case CalculationElementViewMember::CalculationElementView_NetPrice:
		return m_pContent->GetNetPrice().GetColor();
	case CalculationElementViewMember::CalculationElementView_TaxValue:
		return m_pContent->GetTaxValue().GetColor();
	case CalculationElementViewMember::CalculationElementView_GrossPrice:
		return m_pContent->GetGrossPrice().GetColor();
	}

	return Color();
}

int CalculationElementRow::CellImage(uint32_t colId)
{
	return -1;
}

CalculationElementView* CalculationElementRow::Content() const
{
	return m_pContent;
}
