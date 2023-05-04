#pragma once
#include "..\..\ctrls\tabctrl\CustomTabCtrl.h"
#include "..\TabPage.h"
#include "HerdTab.h"

class BarnTab : public TabPage
{
public:
	BarnTab();
	BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
protected:
	CCustomTabCtrl m_HerdsTab;
	std::vector<HerdTab*> m_HerdTabs;

protected:
	
public:
	virtual ~BarnTab();
	
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize( UINT nType,  int cx,  int cy  );	
	afx_msg void OnHerdTabSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHerdTabUnselected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

