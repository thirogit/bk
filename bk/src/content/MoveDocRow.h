#pragma once
#include "../view/MoveDocView.h"
#include "../text/MoveDocViewText.h"
#include "..\ctrls\contentlistctrl\ListRow.h"

class MoveDocRow : public ListRow
{
public:
	MoveDocRow(MoveDocView* pView);
	~MoveDocRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	MoveDocView* Content() const;
private:	
	MoveDocView* m_pRowContent;
	MoveDocViewText m_Text;
};

