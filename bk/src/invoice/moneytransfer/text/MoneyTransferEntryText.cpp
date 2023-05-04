#include "stdafx.h"
#include "MoneyTransferEntryText.h"

std::wstring MoneyTransferEntryText::Text(MoneyTransferView *view,uint32_t memberId)
{
	std::wstring text;
	switch (memberId)
	{
	case MoneyTransferViewMember::MoneyTransferViewMember_HentAlias:
		text = view->GetHentAlias();
		break;
	case MoneyTransferViewMember::MoneyTransferViewMember_InvoiceNo:
		text = view->GetInvoiceNo();
		break;
	case MoneyTransferViewMember::MoneyTransferViewMember_CustomInvoiceNo:
		text = view->GetCustomInvoiceNo();
		break;
	case MoneyTransferViewMember::MoneyTransferViewMember_TransferValue:
		text = view->GetTransferValue().ToString(2);
		break;
	case MoneyTransferViewMember::MoneyTransferViewMember_StatusDesc:
		text = view->GetStatusDesc().Object();
		break;	
	}	
	return text;
}
