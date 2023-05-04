#pragma once
#include "../view/HentsDocView.h"
#include "../text/HentsDocViewText.h"
#include "..\ctrls\contentlistctrl\ListRow.h"

class HentsDocRow : public ListRow
{
public:
	typedef HentsDocView content_type;

	HentsDocRow(HentsDocView* pView);
	~HentsDocRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	HentsDocView* Content() const;

	const Color& GetRowColor();
	void SetRowColor(const Color& color);

private:	
	HentsDocView* m_pRowContent;
	HentsDocViewText m_Text;
	Color m_rowColor;
};

