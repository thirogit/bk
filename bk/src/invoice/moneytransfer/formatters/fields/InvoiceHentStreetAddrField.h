#ifndef __INVOICEHENTSTREETADDRFIELD_H__
#define __INVOICEHENTSTREETADDRFIELD_H__

#include "FormatterField.h"

class InvoiceHentStreetAddrField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif