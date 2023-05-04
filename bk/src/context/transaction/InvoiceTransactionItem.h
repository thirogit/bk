#ifndef __INVOICETRANSACTIONITEM_H__
#define __INVOICETRANSACTIONITEM_H__

#include <data\runtime\InvoiceItemKey.h>
#include <data\impl\CowInvoiceEntryDetails.h>
#include <data\impl\CowDetails.h>
#include <data\impl\CowEntryDetails.h>

class InvoiceTransactionItem
{
public:
	InvoiceTransactionItem(const InvoiceItemKey& itemKey);
	
	const InvoiceItemKey& GetItemKey() const;	
	uint32_t GetCowId() const;	
	const CowDetails& GetCowDetails() const;
	const CowInvoiceEntryDetails& GetEntry() const;
	const CowEntryDetails& GetDocEntry() const;
	
		
	void SetCowDetails(const CowDetails& cow);
	void SetEntry(const CowInvoiceEntryDetails& entry);	
	void SetCowId(uint32_t cowId);
	void SetDocEntry(const CowEntryDetails& docEntry);
	

private:
	InvoiceItemKey m_itemKey;
	uint32_t m_cowId;
	CowDetails m_cowDetails;
	CowInvoiceEntryDetails m_Entry;
	CowEntryDetails m_docEntry;
};

#endif