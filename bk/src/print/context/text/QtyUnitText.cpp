#include "stdafx.h"
#include "QtyUnitText.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"

QtyUnitText::QtyUnitText(QtyUnit unit)
{
	switch (unit)
	{
	case QtyUnit_Kilogram:
		std::wstring::operator=(StringLoader(IDS_QTY_UNIT_KG));
		break;
	case QtyUnit_Piece:
		std::wstring::operator=(StringLoader(IDS_QTY_UNIT_PIECE));
	}
}

const std::wstring& QtyUnitText::ToString()
{
	return *this;
}
