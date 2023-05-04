#include "stdafx.h"
#include "CowPresenceSelectRow.h"
#include "../uiconstants/ColorConstants.h"

CowPresenceSelectRow::CowPresenceSelectRow(CowPresenceView* pContent) : m_pContent(pContent),m_bSelected(false)
{
}

CowPresenceSelectRow::~CowPresenceSelectRow()
{
	delete m_pContent;
}

std::wstring CowPresenceSelectRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pContent,colId);
}

Color CowPresenceSelectRow::CellColor(uint32_t colId)
{
	if(IsSelected())
		return Color(ROW_SELECTED_CLR);

	if(m_pContent->IsLocked())
		return Color(LOCKED_ROW_COLOR);

	return Color();
}

bool CowPresenceSelectRow::IsSelected() const
{
	return m_bSelected;
}

void CowPresenceSelectRow::SetSelected(bool selected)
{
	m_bSelected = selected;
}

CowPresenceView* CowPresenceSelectRow::Content() const
{
	return m_pContent;
}

void CowPresenceSelectRow::SetItemId(ItemId itemId)
{
	m_ItemId = itemId;
}

ItemId CowPresenceSelectRow::GetItemId() const
{
	return m_ItemId;
}

