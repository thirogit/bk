#include "stdafx.h"
#include "InvoiceCowItem.h"


InvoiceCowItem::InvoiceCowItem(uint32_t cowId,uint32_t docId) : m_cowId(cowId),m_docId(docId)
{
}	

InvoiceCowItem::InvoiceCowItem(const InvoiceCowItem& src)
{
	operator=(src);
}

InvoiceCowItem& InvoiceCowItem::operator=(const InvoiceCowItem& right)
{
	m_cowId = right.GetCowId();
	m_cowDetails.CopyFrom(right.m_cowDetails);
	m_entryDetails.CopyFrom(right.m_entryDetails);
	m_docId = right.m_docId;
	return *this;
}
	
uint32_t InvoiceCowItem::GetCowId() const
{
	return m_cowId;
}

uint32_t InvoiceCowItem::GetDocId() const
{
	return m_docId;
}

const CowDetails& InvoiceCowItem::GetCow() const
{
	return m_cowDetails;
}

void InvoiceCowItem::SetCow(const CowDetails& cow)
{
	m_cowDetails.CopyFrom(cow);
}

const CowInvoiceEntryDetails& InvoiceCowItem::GetEntry() const
{
	return m_entryDetails;
}

void InvoiceCowItem::SetEntry(const CowInvoiceEntryDetails& entry)
{
	m_entryDetails.CopyFrom(entry);
}

