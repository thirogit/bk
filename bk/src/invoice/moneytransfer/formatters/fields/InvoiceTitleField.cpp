#include "stdafx.h"
#include "InvoiceTitleField.h"


std::wstring InvoiceTitleField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const InvoiceView* pInvoice = pMoneyTransfer->GetInvoice();

	std::wstring invoiceTitle(L"FAKTURA VAT ");

	if(pInvoice->GetInvoiceType() == InvoiceType_Lump)
		invoiceTitle += L"RR";

	return invoiceTitle;
}