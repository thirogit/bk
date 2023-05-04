#include "stdafx.h"
#include "InvoiceNumberField.h"


std::wstring InvoiceNumberField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const InvoiceView* pInvoice = pMoneyTransfer->GetInvoice();
		
	if(pInvoice->GetCustomNumber().empty())
		return pInvoice->GetInvoiceNo();
	else
		return pInvoice->GetCustomNumber();

}