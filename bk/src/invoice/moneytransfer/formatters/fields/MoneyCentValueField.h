#ifndef __MONEYCENTVALUEFIELD_H__
#define __MONEYCENTVALUEFIELD_H__

#include "FormatterField.h"

class MoneyCentValueField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif