#include "stdafx.h"
#include "OwnDocRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

OwnDocRow::OwnDocRow(OwnDocView* pView) : m_pRowContent(pView)
{
}

OwnDocRow::~OwnDocRow()
{
	delete m_pRowContent;
}

std::wstring OwnDocRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color OwnDocRow::CellColor(uint32_t colId)
{	
	return m_rowColor;
}

int OwnDocRow::CellImage(uint32_t colId)
{
	return -1;
}

OwnDocView* OwnDocRow::Content() const
{
	return m_pRowContent;
}

const Color& OwnDocRow::GetRowColor()
{
	return m_rowColor;
}

void OwnDocRow::SetRowColor(const Color& color)
{
	m_rowColor = color;
}