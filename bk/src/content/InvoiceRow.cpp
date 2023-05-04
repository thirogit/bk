#include "stdafx.h"
#include "InvoiceRow.h"

InvoiceRow::InvoiceRow(InvoiceView* pView) : m_pRowContent(pView)
{
}

InvoiceRow::~InvoiceRow()
{
	delete m_pRowContent;
}

std::wstring InvoiceRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color InvoiceRow::CellColor(uint32_t colId)
{	
	return m_rowColor;
}

int InvoiceRow::CellImage(uint32_t colId)
{
	return -1;
}

InvoiceView* InvoiceRow::Content() const
{
	return m_pRowContent;
}

const Color& InvoiceRow::GetRowColor()
{
	return m_rowColor;
}

void InvoiceRow::SetRowColor(const Color& color)
{
	m_rowColor = color;
}