#include "stdafx.h"
#include "DeductionElementRow.h"
#include "../../uiconstants/PrecisionContstants.h"
#include "../../text/YesNoText.h"

DeductionElementRow::DeductionElementRow(DeductionElementView* pView) : m_pContent(pView)
{
}

DeductionElementRow::~DeductionElementRow()
{
	delete m_pContent;
}

std::wstring DeductionElementRow::CellValue(uint32_t colId)
{
	switch(colId)
	{	
	case DeductionElementViewMember::Enabled:
			return YesNoText(m_pContent->GetIsEnabled()).ToString();
	case DeductionElementViewMember::Code:
			return m_pContent->GetCode();
	case DeductionElementViewMember::Fraction:
			return (m_pContent->GetFraction() * Decimal(100L)).ToString(2) + L"%";
	case DeductionElementViewMember::Reason:
			return m_pContent->GetReason();
	case DeductionElementViewMember::DeductedAmount:
			return m_pContent->GetAmount().ToString(PRICE_PREC);
	}

	return std::wstring();
}

Color DeductionElementRow::CellColor(uint32_t colId)
{
	return Color();
}

int DeductionElementRow::CellImage(uint32_t colId)
{
	if (colId == DeductionElementViewMember::Code)
	{
		return (m_pContent->GetIsEnabled() ? 1 : 0);
	}

	return -1;
}

DeductionElementView* DeductionElementRow::Content() const
{
	return m_pContent;
}
