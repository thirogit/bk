#include "stdafx.h"
#include "InvoiceTransactionCowItem.h"

InvoiceTransactionCowItem::InvoiceTransactionCowItem(const InvoiceItemKey& itemKey,uint32_t cowId) : m_ItemKey(itemKey),InvoiceCowItem(cowId,itemKey.GetDocKey().GetDocNo())
{
}

const InvoiceItemKey& InvoiceTransactionCowItem::GetItemKey() const
{
	return m_ItemKey;
}
