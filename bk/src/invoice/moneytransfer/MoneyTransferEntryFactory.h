#ifndef __MONEYTRANSFERENTRYFACTORY_H__
#define __MONEYTRANSFERENTRYFACTORY_H__

#include "../../context/SeasonSession.h"
#include "MoneyTransferEntry.h"
#include "../../view/InvoiceViewCreator.h"

class MoneyTransferEntryFactory
{
public:
	MoneyTransferEntryFactory(SeasonSession* session);		
	MoneyTransferEntry* CreateEntryForInvoice(uint32_t invoiceId);
protected:
	SeasonSession* m_session;
	InvoiceViewCreator m_viewCreator;
};


#endif