#pragma once
#include "Row.h"
#include "../view/PurchaseHentView.h"
#include "../text/PurchaseHentViewText.h"

class PurchaseHentRow : public Row<PurchaseHentView>
{
public:
	PurchaseHentRow(PurchaseHentView* pView);	
	virtual std::wstring CellValue(uint32_t colId);
	
private:	
	PurchaseHentViewText m_Text;
};

