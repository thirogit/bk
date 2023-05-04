#include "stdafx.h"
#include "InvoiceHentAddrField.h"


std::wstring InvoiceHentAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	std::wstring sHentAddr;
	const IInvoiceHent* pHent = pMoneyTransfer->GetHent();
	sHentAddr = pHent->GetStreet() + L' ' + pHent->GetPOBox() + L", " + pHent->GetCity() + L' ' + pHent->GetZip();
	return sHentAddr;
}

