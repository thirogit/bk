#ifndef __CONSTANTFIELD_H__
#define __CONSTANTFIELD_H__

#include "FormatterField.h"

class ConstantField : public FormatterField
{
public:
	ConstantField(const std::wstring& sFieldValue);
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	std::wstring m_FieldValue;

};


#endif