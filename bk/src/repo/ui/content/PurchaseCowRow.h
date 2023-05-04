#pragma once
#include "Row.h"
#include "../view/PurchaseCowView.h"
#include "../text/PurchaseCowViewText.h"

class PurchaseCowRow : public Row<PurchaseCowView>
{
public:
	PurchaseCowRow(PurchaseCowView* pView);
	virtual std::wstring CellValue(uint32_t colId);
	
private:	
	PurchaseCowViewText m_Text;
};

