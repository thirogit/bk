#include "stdafx.h"
#include "InvoiceSnapshot.h"
#include <arrays/DeepDelete.h>

InvoiceSnapshot::InvoiceSnapshot(uint32_t invoiceId) : m_invoiceId(invoiceId),m_invoicetype(InvoiceType_None)
{
}

InvoiceSnapshot::~InvoiceSnapshot()
{
	DeepDelete(m_items);
	DeepDelete(m_deductions);
}

void InvoiceSnapshot::SetInvoiceType(InvoiceType type)
{
	m_invoicetype = type;
}

InvoiceType InvoiceSnapshot::GetInvoiceType() const
{
	return m_invoicetype;
}

uint32_t InvoiceSnapshot::GetInvoiceId() const
{
	return m_invoiceId;
}

void InvoiceSnapshot::SetDetails(const InvoiceDetails& details)
{
	m_details.CopyFrom(details);
}

const InvoiceDetails& InvoiceSnapshot::GetDetails() const
{
	return m_details;
}

void InvoiceSnapshot::SetInvoiceHent(const InvoiceHent& hent)
{
	m_hent.CopyFrom(&hent);
}

const InvoiceHent& InvoiceSnapshot::GetInvoiceHent() const
{
	return m_hent;
}

const InvoiceCowItem& InvoiceSnapshot::GetItem(int index) const
{
	return *m_items.at(index);
}

void InvoiceSnapshot::AddItem(const InvoiceCowItem& item)
{
	m_items.push_back(new InvoiceCowItem(item));
}

InvoiceSnapshotItemsRange InvoiceSnapshot::Items() const
{
	return InvoiceSnapshotItemsRange(m_items.begin(),m_items.end());
}

const InvoiceDeduction& InvoiceSnapshot::GetDeduction(int index) const
{
	return *m_deductions.at(index);
}

void InvoiceSnapshot::AddDeduction(const InvoiceDeduction& item)
{
	m_deductions.push_back(new InvoiceDeduction(item));
}

InvoiceSnapshotDeductionsRange InvoiceSnapshot::Deductions() const
{
	return InvoiceSnapshotDeductionsRange(m_deductions);
}


	
	
