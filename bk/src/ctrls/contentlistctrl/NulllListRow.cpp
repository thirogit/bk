#include "stdafx.h"
#include "NullListRow.h"

std::wstring NullListRow::CellValue(uint32_t colId)
{
	return L"";
}

Color NullListRow::CellColor(uint32_t colId)
{
	return Color();
}

int NullListRow::CellImage(uint32_t colId)
{
	return -1;
}