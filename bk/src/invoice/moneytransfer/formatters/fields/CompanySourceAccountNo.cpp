#include "stdafx.h"
#include "CompanySourceAccountNo.h"

std::wstring CompanySourceAccountNo::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return pContext->GetSrcAccountNo().GetNumber();
}