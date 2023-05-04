#pragma once

#include "..\ctrls\contentlistctrl\ListRow.h"
#include "../text/HentText.h"
#include "../view/HentView.h"

class HentRow : public ListRow
{
public:
	HentRow(HentView* pView);
	~HentRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	HentView* Content() const;
	bool IsMarked() const;
	void SetMarked(bool bMarked);
private:
	HentText m_Text;
	HentView* m_pRowContent;
	bool m_Marked;
};

