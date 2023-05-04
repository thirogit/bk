#ifndef __QUOTEDFIELD_H__
#define __QUOTEDFIELD_H__

#include "FormatterField.h"

class QuotedField : public FormatterField
{
public:
	QuotedField(FormatterField* pField);
	~QuotedField();
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	FormatterField* m_pField;

};


#endif