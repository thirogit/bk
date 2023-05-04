#pragma once

#include "..\ListTab.h"
#include "../../view/HentView.h"
#include "../../ctrls/xtoolbar/XToolBar.h"
#include "../../header/HeaderLayoutConfiguration.h"
#include "../find/FindContainerDlg.h"
#include "../../content/HentsListContent.h"

class HentsTab : public ListTab
{
public:
	HentsTab();	
	virtual ~HentsTab();
	int FindHents(Predicate<HentView>* predicate);
protected:
	HentsListContent m_ListContent;
	HeaderLayoutConfiguration m_HdrLayout;
	HACCEL  m_hAccelTable;
	FindContainerDlg* m_FindContainer;
	BOOL m_bFindVisible;
protected:
	void OnAddHent();
	void OnFindHent();
	void OnEditHent();
	void OnDeleteHent();	
	void RecalcLayout();
	void OnHentSyncYes();
	void OnHentSyncNo();

	void OnRClick(int row,int cow);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnShowFind();
	afx_msg void OnHideFind();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnFind(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHideFind(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearFound(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSortFound(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

