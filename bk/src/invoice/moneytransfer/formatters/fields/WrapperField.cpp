#include "stdafx.h"
#include "WrapperField.h"


WrapperField::WrapperField(FormatterField* pField) : m_pField(pField)
{
}

WrapperField::~WrapperField()
{
	delete m_pField;
}

std::wstring WrapperField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{	
	return m_pField->GetFieldValue(pContext,pMoneyTransfer);	
}
