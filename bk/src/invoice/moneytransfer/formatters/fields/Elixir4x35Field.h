#ifndef __ELIXIR4X35FIELD_H__
#define __ELIXIR4X35FIELD_H__

#include "FormatterField.h"

class Elixir4x35Field : public FormatterField
{
public:
	Elixir4x35Field(FormatterField* pField);
	~Elixir4x35Field();
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	FormatterField* m_pField;

};


#endif