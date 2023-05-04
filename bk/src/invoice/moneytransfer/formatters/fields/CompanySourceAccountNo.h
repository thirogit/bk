#ifndef __COMPANYSOURCEACCOUNTNO_H__
#define __COMPANYSOURCEACCOUNTNO_H__

#include "FormatterField.h"

class CompanySourceAccountNo : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif