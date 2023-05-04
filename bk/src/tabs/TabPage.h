#pragma once

#include "../SessionAwareWnd.h"

class TabPage : public CSessionAwareWnd<CWnd>
{
public:
	TabPage();
	BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
protected:


protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
			BOOL RegisterWindowClass();

public:
	virtual ~TabPage();
	
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize( UINT nType,  int cx,  int cy  );
	
	DECLARE_MESSAGE_MAP()
};

