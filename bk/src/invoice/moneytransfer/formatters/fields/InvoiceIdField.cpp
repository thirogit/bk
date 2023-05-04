#include "stdafx.h"
#include "InvoiceIdField.h"
#include <string/IntString.h>


std::wstring InvoiceIdField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const InvoiceView* pInvoice = pMoneyTransfer->GetInvoice();	
	return IntString(pInvoice->GetInvoiceId());
}