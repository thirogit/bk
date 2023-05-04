#ifndef __CURRENCYCODEFIELD_H__
#define __CURRENCYCODEFIELD_H__

#include "FormatterField.h"

class CurrencyCodeField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
};


#endif