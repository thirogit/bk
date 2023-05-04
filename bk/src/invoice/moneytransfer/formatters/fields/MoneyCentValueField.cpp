#include "stdafx.h"
#include "MoneyCentValueField.h"
#include <string/IntString.h>

std::wstring MoneyCentValueField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Decimal transferValueMills  = pMoneyTransfer->GetTransferValue()*Decimal(1000L);
	long long mills = transferValueMills.Integer();
	
	long long decimals = mills % 10;
	long long cents = (mills / 10) + (decimals >= 5 ? 1 : 0);

	return IntString(cents);
}