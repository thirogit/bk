#include "stdafx.h"
#include "MoneyTransferTitleField.h"


std::wstring MoneyTransferTitleField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const InvoiceView* pInvoice = pMoneyTransfer->GetInvoice();

	std::wstring transferTitle(L"FAKTURA VAT ");

	if(pInvoice->GetInvoiceType() == InvoiceType_Lump)
		transferTitle += L"RR ";

	if(pInvoice->GetCustomNumber().empty())
		transferTitle += pInvoice->GetInvoiceNo();
	else
		transferTitle += pInvoice->GetCustomNumber();

	transferTitle += L" Z DNIA ";
	transferTitle += pInvoice->GetInvoiceDate().GetDateInFmt(DateTime::YYYYdashMMdashDD);

	return transferTitle;
}