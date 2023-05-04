#ifndef __MONEYFLOATVALUEFIELD_H__
#define __MONEYFLOATVALUEFIELD_H__

#include "FormatterField.h"

class MoneyFloatValueField : public FormatterField
{
public:
	MoneyFloatValueField(const DecimalFormat& decimalFmt);
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:

	DecimalFormat m_decimalFmt;
};


#endif