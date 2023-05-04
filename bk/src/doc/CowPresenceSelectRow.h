#ifndef __COWPRESENCESELECTROW_H__
#define __COWPRESENCESELECTROW_H__

#include "../ctrls/contentlistctrl/NullListRow.h"
#include "../view/CowPresenceView.h"
#include "../text/CowPresenceViewText.h"
#include <data\runtime\ItemId.h>

class CowPresenceSelectRow : public NullListRow
{
public:
	CowPresenceSelectRow(CowPresenceView* pContent);
	~CowPresenceSelectRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);	

	bool IsSelected() const;
	void SetSelected(bool selected);

	//niepodoba mi sie to, do komunikacji z fasada transakcji uzyc cowId 
	ItemId GetItemId() const;
	void SetItemId(ItemId itemId);

	CowPresenceView* Content() const;
private:
	CowPresenceView* m_pContent;
	bool m_bSelected;	
	ItemId m_ItemId;
	CowPresenceViewText m_Text;
};

#endif
