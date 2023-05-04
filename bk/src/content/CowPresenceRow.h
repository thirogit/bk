#pragma once
#include "../view/CowPresenceView.h"
#include "../text/CowPresenceViewText.h"
#include "..\ctrls\contentlistctrl\ListRow.h"

class CowPresenceRow : public ListRow
{
public:
	typedef CowPresenceView content_type;	

	CowPresenceRow(CowPresenceView* pCowView);
	~CowPresenceRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	CowPresenceView* Content() const;
	void SetIsMarked(bool marked);
	bool GetIsMarked() const;

private:	
	CowPresenceView* m_pRowContent;
	CowPresenceViewText m_Text;
	bool            m_Marked;
};

