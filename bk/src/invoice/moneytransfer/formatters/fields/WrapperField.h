#ifndef __WRAPPERFIELD_H__
#define __WRAPPERFIELD_H__

#include "FormatterField.h"

class WrapperField : public FormatterField
{
public:
	WrapperField(FormatterField* pField);
	virtual ~WrapperField();
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	FormatterField* m_pField;

};


#endif