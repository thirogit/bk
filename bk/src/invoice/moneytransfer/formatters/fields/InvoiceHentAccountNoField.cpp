#include "stdafx.h"
#include "InvoiceHentAccountNoField.h"

std::wstring InvoiceHentAccountNoField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const IInvoiceHent* pHent = pMoneyTransfer->GetHent();
	return pHent->GetAccountNo().GetNumber();
}