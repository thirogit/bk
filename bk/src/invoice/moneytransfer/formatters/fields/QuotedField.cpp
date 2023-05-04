#include "stdafx.h"
#include "QuotedField.h"


QuotedField::QuotedField(FormatterField* pField) : m_pField(pField)
{
}

QuotedField::~QuotedField()
{
	delete m_pField;
}

std::wstring QuotedField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const wchar_t cQuote = L'\"';
	std::wstring sFieldValue = m_pField->GetFieldValue(pContext,pMoneyTransfer);
	return  cQuote + sFieldValue + cQuote;
}
