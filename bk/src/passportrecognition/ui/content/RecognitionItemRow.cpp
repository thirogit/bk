#include "stdafx.h"
#include "RecognitionItemRow.h"


RecognitionItemRow::RecognitionItemRow(RecognitionItemView* pItemView) : m_pRowContent(pItemView)
{
}

RecognitionItemRow::~RecognitionItemRow()
{
	delete m_pRowContent;
}

std::wstring RecognitionItemRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color RecognitionItemRow::CellColor(uint32_t colId)
{		
	return Color();
}

int RecognitionItemRow::CellImage(uint32_t colId)
{
	return -1;
}

RecognitionItemView* RecognitionItemRow::Content() const
{
	return m_pRowContent;
}

