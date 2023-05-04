#include "stdafx.h"
#include "ReplaceField.h"



ReplaceRule::ReplaceRule(wchar_t cWhat, wchar_t cWith) : m_cWhat(cWhat), m_cWith(cWith)
{
}

ReplaceRule::ReplaceRule(const ReplaceRule& src)
{
	CopyFrom(src);	
}

ReplaceRule& ReplaceRule::operator=(const ReplaceRule& right)
{
	CopyFrom(right);
	return *this;
}

void ReplaceRule::CopyFrom(const ReplaceRule& src)
{
	m_cWhat = src.m_cWhat;
	m_cWith = src.m_cWith;
}

wchar_t ReplaceRule::ApplyRule(wchar_t ch)
{
	if(ch == m_cWhat)
		return m_cWith;

	return ch;
}

ReplaceRules::ReplaceRules()
{
}

ReplaceRules::ReplaceRules(const ReplaceRules& src)
{
	CopyFrom(src);	
}

ReplaceRules& ReplaceRules::operator=(const ReplaceRules& right)
{
	CopyFrom(right);
	return *this;
}

void ReplaceRules::CopyFrom(const ReplaceRules& src)
{
	m_rules.clear();
	m_rules = src.m_rules;
	
}

ReplaceRules& ReplaceRules::Add(const ReplaceRule& replaceRule)
{
	m_rules.push_back(replaceRule);
	return *this;
}

wchar_t ReplaceRules::ApplyRules(wchar_t ch)
{
	wchar_t chAfterRules = ch;
	std::list<ReplaceRule>::iterator ruleIt = m_rules.begin();
	while(ruleIt != m_rules.end())
	{
		chAfterRules = ruleIt->ApplyRule(chAfterRules);
		if(chAfterRules == L'\0')
			return chAfterRules;
		ruleIt++;
	}

	return chAfterRules;
}


ReplaceField::ReplaceField(FormatterField* pField,const ReplaceRules& rules) : WrapperField(pField),m_replaceRules(rules)
{
}

std::wstring ReplaceField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	std::wstring sValue = WrapperField::GetFieldValue(pContext,pMoneyTransfer);

	std::wstring sResult;
	wchar_t ch;
	for(int i = 0, l = sValue.length();i < l;i++)
	{
		ch = m_replaceRules.ApplyRules(sValue.at(i));
		if (ch != L'\0')
		{
			sResult += ch;
		}
	}

	return sResult;
}

