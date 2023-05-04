#ifndef __COMPANYSOURCEBANKNO_H__
#define __COMPANYSOURCEBANKNO_H__

#include "FormatterField.h"

class CompanySourceBankNo : public FormatterField
{
public:
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif