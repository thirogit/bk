#ifndef __REPLACEFIELD_H__
#define __REPLACEFIELD_H__

#include "WrapperField.h"
#include <list>

class ReplaceRule
{
public:
	ReplaceRule(wchar_t cWhat, wchar_t cWith);
	ReplaceRule(const ReplaceRule& src);
	ReplaceRule& operator=(const ReplaceRule& right);
	void CopyFrom(const ReplaceRule& src);
	wchar_t ApplyRule(wchar_t ch);
private:
	wchar_t m_cWhat;
	wchar_t m_cWith;
};

class ReplaceRules
{
public:
	ReplaceRules();
	ReplaceRules(const ReplaceRules& src);
	ReplaceRules& operator=(const ReplaceRules& right);
	void CopyFrom(const ReplaceRules& src);
	ReplaceRules& Add(const ReplaceRule& replaceRule);
	wchar_t ApplyRules(wchar_t ch);
private:
	std::list<ReplaceRule> m_rules;
};


class ReplaceField : public WrapperField
{
public:
	ReplaceField(FormatterField* pField,const ReplaceRules& rules);
	virtual std::wstring GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	ReplaceRules m_replaceRules;
};


#endif