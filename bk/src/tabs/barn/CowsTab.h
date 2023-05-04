#pragma once
#include "../ListTab.h"
#include "..\..\ctrls\contentlistctrl\ContentListCtrl.h"
#include "../../context/SeasonSession.h"
#include "../../content/CowsListContent.h"
#include "../find/FindContainerDlg.h"
#include "../../content/predicate/Predicate.h"
#include "../find/FindCowDlg.h"

class CowsTab : public ListTab
{
public:
	CowsTab();	
	
protected:
	HACCEL  m_hAccelTable;
	FindContainerDlg* m_FindContainer;
	BOOL m_bFindVisible;
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int Find(Predicate<CowPresenceView>* predicate);
	void RecalcLayout();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnDestroy();
	afx_msg void OnShowFind();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnFind(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHideFind(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnClearFound(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSortFound(WPARAM wParam, LPARAM lParam);

	
	DECLARE_MESSAGE_MAP()
};

