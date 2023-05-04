#ifndef __EMPTYFIELD_H__
#define __EMPTYFIELD_H__

#include "FormatterField.h"

class EmptyField : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif