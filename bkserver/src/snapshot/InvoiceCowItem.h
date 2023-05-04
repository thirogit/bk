#pragma once

#include <data/impl/CowDetails.h>
#include <data/impl/CowInvoiceEntryDetails.h>

class InvoiceCowItem
{
public:
	InvoiceCowItem(uint32_t cowId,uint32_t docId);
	InvoiceCowItem(const InvoiceCowItem& src);
	InvoiceCowItem& operator=(const InvoiceCowItem& right);

	uint32_t GetCowId() const;
	uint32_t GetDocId() const;

	const CowDetails& GetCow() const;
	void SetCow(const CowDetails& cow);
	
	const CowInvoiceEntryDetails& GetEntry() const;
	void SetEntry(const CowInvoiceEntryDetails& entry);	

private:
	uint32_t m_cowId;
	uint32_t m_docId;
	CowDetails m_cowDetails;
	CowInvoiceEntryDetails m_entryDetails;
};
