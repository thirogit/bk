#ifndef __MAXSIZEFIELD_H__
#define __MAXSIZEFIELD_H__

#include "WrapperField.h"

class MaxSizeField : public WrapperField
{
public:
	MaxSizeField(FormatterField* pField,uint32_t uMaxSize);
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	uint32_t m_uMaxSize;
};


#endif