#include "stdafx.h"
#include "HentsDocRow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HentsDocRow::HentsDocRow(HentsDocView* pView) : m_pRowContent(pView)
{
}

HentsDocRow::~HentsDocRow()
{
	delete m_pRowContent;
}

std::wstring HentsDocRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color HentsDocRow::CellColor(uint32_t colId)
{	
	return m_rowColor;
}

int HentsDocRow::CellImage(uint32_t colId)
{
	return -1;
}

HentsDocView* HentsDocRow::Content() const
{
	return m_pRowContent;
}

const Color& HentsDocRow::GetRowColor()
{
	return m_rowColor;
}

void HentsDocRow::SetRowColor(const Color& color)
{
	m_rowColor = color;
}