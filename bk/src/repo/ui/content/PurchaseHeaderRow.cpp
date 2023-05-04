#include "stdafx.h"
#include "PurchaseHeaderRow.h"


static Color _downloadedColor(255, 193, 193);

PurchaseHeaderRow::PurchaseHeaderRow(PurchaseHeaderView* pView) : m_pRowContent(pView)
{
}

PurchaseHeaderRow::~PurchaseHeaderRow()
{
	delete m_pRowContent;
}

std::wstring PurchaseHeaderRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color PurchaseHeaderRow::CellColor(uint32_t colId)
{	
	if (m_pRowContent->GetDownloaded())
		return _downloadedColor;

	return Color();
}

int PurchaseHeaderRow::CellImage(uint32_t colId)
{
	return -1;
}

PurchaseHeaderView* PurchaseHeaderRow::Content() const
{
	return m_pRowContent;
}

