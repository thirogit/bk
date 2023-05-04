#pragma once
#include "../view/InvoiceView.h"
#include "../text/InvoiceViewText.h"
#include "..\ctrls\contentlistctrl\ListRow.h"

class InvoiceRow : public ListRow
{
public:
	typedef InvoiceView content_type;

	InvoiceRow(InvoiceView* pView);
	~InvoiceRow();
	
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	
	InvoiceView* Content() const;

	const Color& GetRowColor();
	void SetRowColor(const Color& color);
private:	
	InvoiceView* m_pRowContent;
	InvoiceViewText m_Text;
	Color m_rowColor;
};

