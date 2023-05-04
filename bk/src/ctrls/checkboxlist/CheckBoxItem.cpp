#include "stdafx.h"
#include "CheckBoxItem.h"

CheckBoxItem::CheckBoxItem(uint32_t optionId,const std::wstring& optionDesc,bool bOptionValue) :
	m_sOptionDesc(optionDesc),m_bOptionValue(bOptionValue),m_OptionId(optionId)
{
}

const std::wstring& CheckBoxItem::GetOptionDesc() const
{
	return m_sOptionDesc;
}

bool CheckBoxItem::GetOptionValue() const
{
	return m_bOptionValue;
}

void CheckBoxItem::SetOptionDesc(const std::wstring& sOptDesc)
{
	m_sOptionDesc = sOptDesc;
}

void CheckBoxItem::SetOptionValue(bool bValue)
{
	m_bOptionValue = bValue;
}

uint32_t CheckBoxItem::GetOptionId() const
{
	return m_OptionId;
}
