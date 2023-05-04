#include "stdafx.h"
#include "MoneyTransferEntryFactory.h"


MoneyTransferEntryFactory::MoneyTransferEntryFactory(SeasonSession* session) : 
	m_session(session),m_viewCreator(session)
{
}

MoneyTransferEntry* MoneyTransferEntryFactory::CreateEntryForInvoice(uint32_t invoiceId)
{
	MoneyTransferEntry* entry = new MoneyTransferEntry();

	BuyInvoiceClientObject* pBuyInvoiceCO = m_session->GetBuyInvoice(invoiceId);
	BuyInvoice* pInvoice = pBuyInvoiceCO->Object();	

	entry->m_invoice = m_viewCreator.CreateView(pInvoice);
	entry->m_hent = new InvoiceHent();
	entry->m_hent->CopyFrom(pInvoice->GetInvoiceHent());
	
	entry->m_status = MoneyTransferEntryStatus_Valid;

	if (pInvoice->GetPayWay() != PayWay_Transfer)
		entry->m_status = MoneyTransferEntryStatus_PaidByCash;
	else if (!pInvoice->GetPaidDate().IsNull())
		entry->m_status = MoneyTransferEntryStatus_AlreadyPaid;
	else if (pInvoice->GetInvoiceHent()->GetAccountNo().IsNull())
		entry->m_status = MoneyTransferEntryStatus_HentHasNoAccountNo;
	
	return entry;

}

