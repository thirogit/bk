#include "stdafx.h"
#include "MaxSizeField.h"


MaxSizeField::MaxSizeField(FormatterField* pField,uint32_t uMaxSize) : WrapperField(pField),m_uMaxSize(uMaxSize)
{
}

std::wstring MaxSizeField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	std::wstring sValue = WrapperField::GetFieldValue(pContext,pMoneyTransfer);
	return sValue.substr(0,m_uMaxSize);
}
