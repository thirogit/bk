#include "stdafx.h"
#include "CowPresenceRow.h"
#include "../uiconstants/ColorConstants.h"

CowPresenceRow::CowPresenceRow(CowPresenceView* pCowView) : m_pRowContent(pCowView), m_Marked(false)
{
}

CowPresenceRow::~CowPresenceRow()
{
	delete m_pRowContent;
}

std::wstring CowPresenceRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color CowPresenceRow::CellColor(uint32_t colId)
{
	if (m_Marked && m_pRowContent->IsLocked())
		return Color(HLIGHTLOCKED);

	if (m_Marked)
		return Color(HLIGHTCOLOR);

	if (m_pRowContent->IsLocked())
		return Color(LOCKED_ROW_COLOR);


	switch(colId)
	{
	case CowPresenceViewMember::State:
			return 	Color(m_pRowContent->IsInStock() ? COW_INSTOCK_CLR : COW_NOTINSTOCK_CLR);
	}
	return Color();
}

int CowPresenceRow::CellImage(uint32_t colId)
{
	return -1;
}

CowPresenceView* CowPresenceRow::Content() const
{
	return m_pRowContent;
}

void CowPresenceRow::SetIsMarked(bool marked)
{
	m_Marked = marked;
}

bool CowPresenceRow::GetIsMarked() const
{
	return m_Marked;
}