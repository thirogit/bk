#include "stdafx.h"
#include "InvoiceHentStreetAddrField.h"


std::wstring InvoiceHentStreetAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const IInvoiceHent* pHent = pMoneyTransfer->GetHent();
	return pHent->GetStreet() + L' ' + pHent->GetPOBox();
}

