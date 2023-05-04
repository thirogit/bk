#ifndef __FORMATTERFIELD_H__
#define __FORMATTERFIELD_H__

#include "../../MoneyTransferEntry.h"
#include "../../MoneyTransferContext.h"
#include <string>

class FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer) = 0;
	virtual ~FormatterField()  = 0;
};


#endif