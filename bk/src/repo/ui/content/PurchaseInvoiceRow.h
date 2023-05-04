#pragma once
#include "../view/PurchaseInvoiceView.h"
#include "../text/PurchaseInvoiceViewText.h"
#include "Row.h"

class PurchaseInvoiceRow : public Row<PurchaseInvoiceView>
{
public:
	PurchaseInvoiceRow(PurchaseInvoiceView* pView);	
	virtual std::wstring CellValue(uint32_t colId);	
private:	
	PurchaseInvoiceViewText m_Text;	
};

