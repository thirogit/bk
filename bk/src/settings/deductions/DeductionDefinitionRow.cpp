#include "stdafx.h"
#include "DeductionDefinitionRow.h"
#include "../../uiconstants/PrecisionContstants.h"
#include "../../text/YesNoText.h"

DeductionDefinitionRow::DeductionDefinitionRow(DeductionDefinitionView* pView) : m_pContent(pView)
{
}

DeductionDefinitionRow::~DeductionDefinitionRow()
{
	delete m_pContent;
}

std::wstring DeductionDefinitionRow::CellValue(uint32_t colId)
{
	switch(colId)
	{	
	case DeductionDefinitionViewMember::Enabled:
			return YesNoText(m_pContent->GetIsEnabled()).ToString();
	case DeductionDefinitionViewMember::Code:
			return m_pContent->GetCode();
	case DeductionDefinitionViewMember::Fraction:
			return (m_pContent->GetFraction() * Decimal(100L)).ToString(2) + L"%";
	case DeductionDefinitionViewMember::Reason:
			return m_pContent->GetReason();
	}

	return std::wstring();
}

Color DeductionDefinitionRow::CellColor(uint32_t colId)
{
	return Color();
}

int DeductionDefinitionRow::CellImage(uint32_t colId)
{
	if (colId == DeductionDefinitionViewMember::Code)
	{
		return (m_pContent->GetIsEnabled() ? 1 : 0);
	}

	return -1;
}

DeductionDefinitionView* DeductionDefinitionRow::Content() const
{
	return m_pContent;
}
