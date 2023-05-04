#pragma once
#include "Row.h"
#include "../view/PurchaseInvoiceDeductionView.h"
#include "../text/PurchaseInvoiceDeductionViewText.h"

class PurchaseInvoiceDeductionRow : public Row<PurchaseInvoiceDeductionView>
{
public:
	PurchaseInvoiceDeductionRow(PurchaseInvoiceDeductionView* pView);	
	virtual std::wstring CellValue(uint32_t colId);
	
private:	
	PurchaseInvoiceDeductionViewText m_Text;
};

