#include "stdafx.h"
#include "InvoiceDateField.h"


InvoiceDateField::InvoiceDateField(DateTime::DateFormat dtFmt) : m_dtFmt(dtFmt)
{
}

std::wstring InvoiceDateField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const InvoiceView* pInvoice = pMoneyTransfer->GetInvoice();
	return pInvoice->GetInvoiceDate().GetDateInFmt(m_dtFmt);
}