#ifndef __INVOICEHENTNAMEANDADDRFIELD_H__
#define __INVOICEHENTNAMEANDADDRFIELD_H__

#include "FormatterField.h"

class InvoiceHentNameAndAddrField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif