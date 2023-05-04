#include "stdafx.h"
#include "InvoiceHentNameField.h"

std::wstring InvoiceHentNameField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{	
	const IInvoiceHent *pHent = pMoneyTransfer->GetHent();
	return pHent->GetName();
}