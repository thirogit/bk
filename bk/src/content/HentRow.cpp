#include "stdafx.h"
#include "HentRow.h"
#include "../uiconstants/ColorConstants.h"

HentRow::HentRow(HentView* pView) : m_pRowContent(pView), m_Marked(false)
{
}

HentRow::~HentRow()
{
	delete m_pRowContent;
}

bool HentRow::IsMarked() const
{
	return m_Marked;
}

void HentRow::SetMarked(bool bMarked)
{
	m_Marked = bMarked;
}

std::wstring HentRow::CellValue(uint32_t colId)
{	
	return m_Text.Text(m_pRowContent,colId);
}

Color HentRow::CellColor(uint32_t colId)
{
	if (m_Marked && m_pRowContent->IsLocked())
		return Color(HLIGHTLOCKED);

	if (m_Marked)
		return Color(HLIGHTCOLOR);

	if(m_pRowContent->IsLocked())
		return Color(LOCKED_ROW_COLOR);

	return Color();
}

int HentRow::CellImage(uint32_t colId)
{
	return -1;
}

HentView* HentRow::Content() const
{
	return m_pRowContent;
}

