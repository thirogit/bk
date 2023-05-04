#include "stdafx.h"
#include "MoneyTransferEntry.h"


MoneyTransferEntry::MoneyTransferEntry() : m_invoice(NULL),m_hent(NULL),m_status(MoneyTransferEntryStatus_None)
{
}

MoneyTransferEntry::~MoneyTransferEntry()
{
	delete m_invoice;
	delete m_hent;
}
		
uint32_t MoneyTransferEntry::GetInvoiceId() const
{
	return m_invoice->GetInvoiceId();
}

const IInvoiceHent* MoneyTransferEntry::GetHent() const
{
	return m_hent;
}

const InvoiceView* MoneyTransferEntry::GetInvoice() const
{
	return m_invoice;
}

MoneyTransferEntryStatus MoneyTransferEntry::GetStatus() const
{
	return m_status;
}

Decimal MoneyTransferEntry::GetTransferValue() const
{
	return m_invoice->GetGrossAfterDeductions();
}
	