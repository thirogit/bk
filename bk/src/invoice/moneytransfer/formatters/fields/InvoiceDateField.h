#ifndef __INVOICEDATEFIELD_H__
#define __INVOICEDATEFIELD_H__

#include "FormatterField.h"

class InvoiceDateField : public FormatterField
{
public:	
	InvoiceDateField(DateTime::DateFormat dtFmt);
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	DateTime::DateFormat m_dtFmt;

};


#endif