#include "stdafx.h"
#include "MoneyTransferDateField.h"


MoneyTransferDateField::MoneyTransferDateField(DateTime::DateFormat dtFmt) :m_dtFmt(dtFmt)
{
}

std::wstring MoneyTransferDateField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return  pContext->GetTransferDate().GetDateInFmt(m_dtFmt);
}

