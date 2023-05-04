#include "stdafx.h"
#include "DocItemRow.h"
#include "../uiconstants/ColorConstants.h"

DocItemRow::DocItemRow(DocItemView* pItemView) : m_pRowContent(pItemView)
{
}

DocItemRow::~DocItemRow()
{
	delete m_pRowContent;
}

std::wstring DocItemRow::CellValue(uint32_t colId)
{
	return m_Text.Text(m_pRowContent,colId);
}

Color DocItemRow::CellColor(uint32_t colId)
{
	switch(colId)
	{
		case DocItemViewMember::DocItemView_Weight:
			return m_pRowContent->GetWeight().GetColor(); 
		case DocItemViewMember::DocItemView_Stock:
			return m_pRowContent->GetStock().GetColor(); 
		case DocItemViewMember::DocItemView_Class:
			return m_pRowContent->GetClass().GetColor(); 
	}
			
	return Color();
}

int DocItemRow::CellImage(uint32_t colId)
{
	return -1;
}

DocItemView* DocItemRow::Content() const
{
	return m_pRowContent;
}

