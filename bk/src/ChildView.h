#pragma once

#include "ctrls\XTabCtrl\XTabCtrl.h"
#include "tabs\barn\BarnTab.h"
#include "tabs\hents\HentsTab.h"
#include "tabs\invoices\InvoicesTab.h"
#include "SessionAwareWnd.h"

class CChildView : public CSessionAwareWnd<CWnd>
{
// Construction
public:
	CChildView();

protected:
	CSessionAwareWnd<CMFCTabCtrl> m_MainTabCtrl;
	BarnTab   m_BarnTab;
	HentsTab  m_HentsTab;
	InvoicesTab m_InvoicesTab;
// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize( UINT nType,  int cx,  int cy  );
	DECLARE_MESSAGE_MAP()
};

