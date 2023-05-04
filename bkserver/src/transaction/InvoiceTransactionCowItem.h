#pragma once

#include "../snapshot/InvoiceCowItem.h"
#include <data/runtime/InvoiceItemKey.h>
#include <data/impl/CowEntryDetails.h>

class InvoiceTransactionCowItem : public InvoiceCowItem
{
public:
	InvoiceTransactionCowItem(const InvoiceItemKey& itemKey,uint32_t cowId);
	const InvoiceItemKey& GetItemKey() const;
		
	void SetDocEntry(const CowEntryDetails& docEntry);
	const CowEntryDetails& GetDocEntry() const;


private:
	InvoiceItemKey m_ItemKey;
	CowEntryDetails m_docEntry;
};
