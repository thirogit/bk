#include "stdafx.h"
#include "InvoiceHentCityField.h"


std::wstring InvoiceHentCityField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return pMoneyTransfer->GetHent()->GetCity();
}

