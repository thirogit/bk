#include "stdafx.h"
#include "MoneyTransferViewCreator.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"
#include "../../calculator/InvoiceCalculator.h"

MoneyTransferViewCreator::MoneyTransferViewCreator()
{
}

MoneyTransferView* MoneyTransferViewCreator::CreateView(const MoneyTransferEntry* entry)
{
	MoneyTransferView* view = new MoneyTransferView(entry->GetInvoiceId());
	RefreshView(view, entry);
	return view;
}

void MoneyTransferViewCreator::RefreshView(MoneyTransferView* pView, const MoneyTransferEntry* entry)
{
	const InvoiceView* invoice = entry->GetInvoice();
	pView->SetCustomInvoiceNo(invoice->GetCustomNumber());
	pView->SetHentAlias(invoice->GetInvoiceHent());
	pView->SetInvoiceNo(invoice->GetInvoiceNo());
	pView->SetTransferValue(invoice->GetGrossAfterDeductions());

	Color errStatus(255, 0, 0);

	switch (entry->GetStatus())
	{
		case MoneyTransferEntryStatus_AlreadyPaid:
			pView->SetStatusDesc(ColoredObject<std::wstring>(StringLoader(IDS_STATUSALREADYPAID), errStatus));
			break;
		
		case MoneyTransferEntryStatus_HentHasNoAccountNo:
			pView->SetStatusDesc(ColoredObject<std::wstring>(StringLoader(IDS_STATUSHENTHASNOACCOUNTNO), errStatus));
			break;

		case MoneyTransferEntryStatus_PaidByCash:
			pView->SetStatusDesc(ColoredObject<std::wstring>(StringLoader(IDS_STATUSPAIDBYCASH), errStatus));
			break;

		default:
			pView->SetStatusDesc(ColoredObject<std::wstring>(L""));
			break;
			
	}
		
	
}

