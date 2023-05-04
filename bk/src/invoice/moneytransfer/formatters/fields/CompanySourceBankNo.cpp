#include "stdafx.h"
#include "CompanySourceBankNo.h"
#include "../utils/AccountNoUtils.h"

std::wstring CompanySourceBankNo::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	AccountNo accountNo = pContext->GetSrcAccountNo();
	return AccountNoUtils::GetBankNo(accountNo);	
}