#ifndef __OWNRDRWLISTCTRL_H__
#define __OWNRDRWLISTCTRL_H__

#include "../../ctrls/contentlistctrl/ContentListCtrl.h"

class COwnrDrwListCtrl : public CContentListCtrl
{
public:
	COwnrDrwListCtrl();
	void SetItemHeight(int itemH);
	
	void SelectRow(int row);
	void UnselectRow(int row);

	void SelectAll();
	void UnselectAll();


protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	int m_ItemHeight;

private:
	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );


	DECLARE_MESSAGE_MAP()	
};

#endif