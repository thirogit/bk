#pragma once
#include "../view/PurchaseHeaderView.h"
#include "../text/PurchaseHeaderViewText.h"
#include "../../../ctrls\contentlistctrl\ListRow.h"

class PurchaseHeaderRow : public ListRow
{
public:
	typedef PurchaseHeaderView content_type;

	PurchaseHeaderRow(PurchaseHeaderView* pView);
	~PurchaseHeaderRow();
	
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	
	PurchaseHeaderView* Content() const;
private:	
	PurchaseHeaderView * m_pRowContent;
	PurchaseHeaderViewText m_Text;
};

