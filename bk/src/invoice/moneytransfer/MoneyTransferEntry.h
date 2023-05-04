#ifndef __MONEYTRANSFERENTRY_H__
#define __MONEYTRANSFERENTRY_H__

#include "../../view/HentView.h"
#include "../../view/InvoiceView.h"
#include <data\impl\InvoiceHent.h>

enum MoneyTransferEntryStatus
{
	MoneyTransferEntryStatus_None,
	MoneyTransferEntryStatus_Valid,
	MoneyTransferEntryStatus_AlreadyPaid,
	MoneyTransferEntryStatus_HentHasNoAccountNo,
	MoneyTransferEntryStatus_PaidByCash,
	MoneyTransferEntryStatus_HasCorrect	
};

class MoneyTransferEntryFactory;

class MoneyTransferEntry
{
	friend class MoneyTransferEntryFactory;
	
	MoneyTransferEntry();
	
public:
	~MoneyTransferEntry();

	uint32_t GetInvoiceId() const;
	const IInvoiceHent* GetHent() const;
	const InvoiceView* GetInvoice() const;
	MoneyTransferEntryStatus GetStatus() const;
	Decimal GetTransferValue() const;
	
protected:
	InvoiceView* m_invoice;
	InvoiceHent* m_hent;
	MoneyTransferEntryStatus m_status;
	
};


#endif