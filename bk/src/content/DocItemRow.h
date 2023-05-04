#pragma once

#include "../view/DocItemView.h"
#include "../text/DocItemViewText.h"
#include "..\ctrls\contentlistctrl\ListRow.h"

class DocItemRow : public ListRow
{
public:
	DocItemRow(DocItemView* pItemView);
	~DocItemRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	DocItemView* Content() const;
private:	
	DocItemView* m_pRowContent;
	DocItemViewText m_Text;
};

