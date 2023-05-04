#include "stdafx.h"
#include "MoveDocRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MoveDocRow::MoveDocRow(MoveDocView* pView) : m_pRowContent(pView)
{
}

MoveDocRow::~MoveDocRow()
{
	delete m_pRowContent;
}

std::wstring MoveDocRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color MoveDocRow::CellColor(uint32_t colId)
{	
	return Color();
}

int MoveDocRow::CellImage(uint32_t colId)
{
	return -1;
}

MoveDocView* MoveDocRow::Content() const
{
	return m_pRowContent;
}