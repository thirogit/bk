#include "stdafx.h"
#include "InvoiceHentZipField.h"


std::wstring InvoiceHentZipField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const IInvoiceHent* pHent = pMoneyTransfer->GetHent();
	return pHent->GetZip();
}

