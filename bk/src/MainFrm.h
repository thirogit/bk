#pragma once

#include "ChildFrm.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	CChildFrame* CreateSessionFrame(SeasonSession* pSession,UINT nResources, HMENU hMenu  = NULL , HACCEL hAccel  = NULL );
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CMFCMenuBar			m_wndMenuBar;
	//CMFCToolBar       m_wndToolBar;
	CMFCStatusBar		m_wndStatusBar;
	CMFCToolBarImages	m_UserImages;

	HICON				m_hIconNoBarcode;
	HICON				m_hIconYesBarcode;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnUpdateBarcodeScannerStatus(CCmdUI * pCmdUI);
	DECLARE_MESSAGE_MAP()

};


