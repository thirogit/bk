#include "stdafx.h"
#include "InvoiceHentNameAndNoField.h"

std::wstring InvoiceHentNameAndNoField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const IInvoiceHent *pInvHent = pMoneyTransfer->GetHent();
	return pInvHent->GetName() + L'(' + pInvHent->GetHentNo().ToString() + L')';
}