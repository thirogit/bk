#include "stdafx.h"
#include "MoneyTransferRow.h"
#include "../../../uiconstants/ColorConstants.h"

MoneyTransferRow::MoneyTransferRow(MoneyTransferView* pCowView) : m_pRowContent(pCowView)
{
}

MoneyTransferRow::~MoneyTransferRow()
{
	delete m_pRowContent;
}

std::wstring MoneyTransferRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color MoneyTransferRow::CellColor(uint32_t colId)
{
	if (colId == MoneyTransferViewMember::MoneyTransferViewMember_StatusDesc)
		return m_pRowContent->GetStatusDesc().GetColor();

	return Color();
}

int MoneyTransferRow::CellImage(uint32_t colId)
{
	return -1;
}

MoneyTransferView* MoneyTransferRow::Content() const
{
	return m_pRowContent;
}

