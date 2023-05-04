#include "stdafx.h"
#include "Elixir4x35Field.h"
#include <string\TextUtils.h>

Elixir4x35Field::Elixir4x35Field(FormatterField* pField) : m_pField(pField)
{
}

Elixir4x35Field::~Elixir4x35Field()
{
	delete m_pField;
}

std::wstring Elixir4x35Field::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	std::wstring s4x35Value;

	std::wstring sValue = TextUtils::Remove(m_pField->GetFieldValue(pContext,pMoneyTransfer),L'|');
	
	std::wstring sLine;
	int iFirst = 0;
	for(int i = 0; i < 4;i++)
	{
		if(iFirst >= sValue.length())
			break;
		sLine = sValue.substr(iFirst,35);

		s4x35Value += sLine;

		iFirst += sLine.length();

		if(iFirst < sValue.length())
			s4x35Value += L'|';
	}
	return s4x35Value;
}

