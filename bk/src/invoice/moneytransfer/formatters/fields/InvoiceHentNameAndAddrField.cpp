#include "stdafx.h"
#include "InvoiceHentNameAndAddrField.h"


std::wstring InvoiceHentNameAndAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const IInvoiceHent *pInvHent = pMoneyTransfer->GetHent();
	std::wstring sHentNameAndAddr = pInvHent->GetName() + L" (" + pInvHent->GetHentNo().ToString() + L") " + pInvHent->GetStreet() + L' ' + pInvHent->GetPOBox() + L", " + pInvHent->GetCity() + L' ' + pInvHent->GetZip();
	return sHentNameAndAddr;
}

