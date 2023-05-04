#include "stdafx.h"
#include "InvoiceTransactionItem.h"
#include <data\IIdable.h>

InvoiceTransactionItem::InvoiceTransactionItem(const InvoiceItemKey& itemKey) : m_itemKey(itemKey), m_cowId(NULL_ID)
{
}


const InvoiceItemKey& InvoiceTransactionItem::GetItemKey() const
{
	return m_itemKey;
}

void InvoiceTransactionItem::SetCowDetails(const CowDetails& details)
{
	m_cowDetails.CopyFrom(details);
}

void InvoiceTransactionItem::SetEntry(const CowInvoiceEntryDetails& entry)
{
	m_Entry.CopyFrom(entry);
}

const CowDetails& InvoiceTransactionItem::GetCowDetails() const
{
	return m_cowDetails;
}

const CowInvoiceEntryDetails& InvoiceTransactionItem::GetEntry() const
{
	return m_Entry;
}

void InvoiceTransactionItem::SetCowId(uint32_t cowId)
{
	m_cowId = cowId;
}

uint32_t InvoiceTransactionItem::GetCowId() const
{
	return m_cowId;
}

const CowEntryDetails& InvoiceTransactionItem::GetDocEntry() const
{
	return m_docEntry;
}

void InvoiceTransactionItem::SetDocEntry(const CowEntryDetails& docEntry)
{
	m_docEntry.CopyFrom(docEntry);
}