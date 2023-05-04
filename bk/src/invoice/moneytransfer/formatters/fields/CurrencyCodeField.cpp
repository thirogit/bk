#include "stdafx.h"
#include "CurrencyCodeField.h"

std::wstring CurrencyCodeField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return L"PLN";//pMoneyTransfer->GetExchangeRate()->GetCountry()->GetCurrencyCode().c_str();
}