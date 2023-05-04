#include "stdafx.h"
#include "PurchaseInvoiceRow.h"

PurchaseInvoiceRow::PurchaseInvoiceRow(PurchaseInvoiceView* pView) : Row<PurchaseInvoiceView>(pView)
{

}

std::wstring PurchaseInvoiceRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent, colId);
}