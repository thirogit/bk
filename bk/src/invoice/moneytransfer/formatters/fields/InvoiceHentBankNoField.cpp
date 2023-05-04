#include "stdafx.h"
#include "InvoiceHentBankNoField.h"
#include "../utils/AccountNoUtils.h"

std::wstring InvoiceHentBankNoField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const IInvoiceHent* pHent = pMoneyTransfer->GetHent();
	return AccountNoUtils::GetBankNo(pHent->GetAccountNo());
}