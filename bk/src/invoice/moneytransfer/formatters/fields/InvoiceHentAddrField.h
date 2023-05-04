#ifndef __INVOICEHENTADDRFIELD_H__
#define __INVOICEHENTADDRFIELD_H__

#include "FormatterField.h"

class InvoiceHentAddrField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif