#ifndef __INVOICENUMBERFIELD_H__
#define __INVOICENUMBERFIELD_H__

#include "FormatterField.h"

class InvoiceNumberField : public FormatterField
{
public:	
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif