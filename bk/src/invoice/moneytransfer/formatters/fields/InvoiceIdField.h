#ifndef __INVOICEIDFIELD_H__
#define __INVOICEIDFIELD_H__

#include "FormatterField.h"

class InvoiceIdField : public FormatterField
{
public:	
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif