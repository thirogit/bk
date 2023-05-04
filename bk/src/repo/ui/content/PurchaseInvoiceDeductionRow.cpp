#include "stdafx.h"
#include "PurchaseInvoiceDeductionRow.h"

PurchaseInvoiceDeductionRow::PurchaseInvoiceDeductionRow(PurchaseInvoiceDeductionView* pView) : Row<PurchaseInvoiceDeductionView>(pView)
{

}

std::wstring PurchaseInvoiceDeductionRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent, colId);
}
