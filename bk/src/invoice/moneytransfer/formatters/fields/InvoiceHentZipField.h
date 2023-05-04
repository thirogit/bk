#ifndef __INVOICEHENTZIPFIELD_H__
#define __INVOICEHENTZIPFIELD_H__

#include "FormatterField.h"

class InvoiceHentZipField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif