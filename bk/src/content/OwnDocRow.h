#pragma once
#include "../view/OwnDocView.h"
#include "../text/OwnDocViewText.h"
#include "..\ctrls\contentlistctrl\ListRow.h"
#include "../uitypes/Color.h"

class OwnDocRow : public ListRow
{
public:
	OwnDocRow(OwnDocView* pView);
	~OwnDocRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	OwnDocView* Content() const;

	const Color& GetRowColor();
	void SetRowColor(const Color& color);
private:	
	OwnDocView* m_pRowContent;
	OwnDocViewText m_Text;
	Color m_rowColor;
};

