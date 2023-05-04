#include "stdafx.h"
#include "MoneyFloatValueField.h"

MoneyFloatValueField::MoneyFloatValueField(const DecimalFormat& decimalFmt) : m_decimalFmt(decimalFmt)
{
}

std::wstring MoneyFloatValueField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Decimal transferValue  = pMoneyTransfer->GetTransferValue();
	return transferValue.ToString(m_decimalFmt);
}