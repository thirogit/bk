#include "stdafx.h"
#include "PurchaseHentRow.h"

PurchaseHentRow::PurchaseHentRow(PurchaseHentView* pView) : Row<PurchaseHentView>(pView)
{

}

std::wstring PurchaseHentRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent, colId);
}
