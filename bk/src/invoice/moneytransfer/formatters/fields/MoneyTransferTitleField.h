#ifndef __MONEYTRANSFERTITLEFIELD_H__
#define __MONEYTRANSFERTITLEFIELD_H__

#include "FormatterField.h"

class MoneyTransferTitleField : public FormatterField
{
public:	
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif