#include "stdafx.h"
#include "DocCowItem.h"
#include <sstream>
#include <string/ToString.h>

DocCowItem::DocCowItem(uint32_t cowId) : m_cowId(cowId)
{
}	

DocCowItem::DocCowItem(const DocCowItem& src) : m_cowId(src.m_cowId)
{
	CopyFrom(src);
}
	
uint32_t DocCowItem::GetCowId() const
{
	return m_cowId;
}

const CowDetails& DocCowItem::GetCow() const
{
	return m_cowDetails;
}

void DocCowItem::SetCow(const CowDetails& cow)
{
	m_cowDetails.CopyFrom(cow);
}

const CowEntryDetails& DocCowItem::GetEntry() const
{
	return m_cowEntryDetails;
}

void DocCowItem::SetEntry(const CowEntryDetails& entry)
{
	m_cowEntryDetails.CopyFrom(entry);
}

void DocCowItem::CopyFrom(const DocCowItem& src)
{
	m_cowDetails.CopyFrom(src.m_cowDetails);
	m_cowEntryDetails.CopyFrom(src.m_cowEntryDetails);
}

std::wstring DocCowItem::ToString() const
{
	std::wstringstream s;
	
	s << L"cow={" << ::ToString(m_cowDetails) << L"}, entry={" << ::ToString(m_cowEntryDetails) << L"}";
	
	return s.str();
}
