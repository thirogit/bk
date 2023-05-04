#ifndef __INVOICEHENTNAMEANDNOFIELD_H__
#define __INVOICEHENTNAMEANDNOFIELD_H__

#include "FormatterField.h"

class InvoiceHentNameAndNoField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
};


#endif