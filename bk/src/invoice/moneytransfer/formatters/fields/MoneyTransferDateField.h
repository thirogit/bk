#ifndef __MONEYRANSFERDATEFIELD_H__
#define __MONEYRANSFERDATEFIELD_H__

#include "FormatterField.h"
#include <types/DateTime.h>

class MoneyTransferDateField : public FormatterField
{
public:
	MoneyTransferDateField(DateTime::DateFormat dtFmt);
	
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	DateTime::DateFormat m_dtFmt;

};


#endif