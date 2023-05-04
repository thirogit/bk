#include "stdafx.h"
#include "PurchaseCowRow.h"

PurchaseCowRow::PurchaseCowRow(PurchaseCowView* pView) : Row<PurchaseCowView>(pView)
{

}

std::wstring PurchaseCowRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent, colId);
}
