#include "stdafx.h"
#include "ColumnLayout.h"


ColumnLayout::ColumnLayout(uint32_t colId,int colWidth) : m_ColWidth(colWidth),m_ColId(colId)
{
}

ColumnLayout::ColumnLayout(const ColumnLayout& layout)
{
	m_ColWidth = layout.m_ColWidth;
	m_ColId = layout.m_ColId;
}

int ColumnLayout::GetColWidth() const
{
	return m_ColWidth;
}

uint32_t ColumnLayout::GetColId() const
{
	return m_ColId;
}

void ColumnLayout::SetColWidth(int width)
{
	m_ColWidth = width;
}