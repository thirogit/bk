#include "stdafx.h"
#include "HeaderColumn.h"

HeaderColumn::HeaderColumn(uint32_t colId,const std::wstring& colText) : m_ColId(colId),m_ColText(colText)
{
}

HeaderColumn::HeaderColumn(const HeaderColumn& src)
{
	operator=(src);
}

HeaderColumn& HeaderColumn::operator=(const HeaderColumn& right)
{
	m_ColId = right.m_ColId;
	m_ColText = right.m_ColText;
	return *this;
}

uint32_t HeaderColumn::GetColId() const
{
	return m_ColId; 
}

const std::wstring& HeaderColumn::GetColText() const
{
	return m_ColText;
}
