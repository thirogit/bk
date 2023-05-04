
#pragma once
#include "ChildView.h"
#include "context\SeasonSession.h"
#include <set>
#include "context/HerdAudience.h"

class CChildFrame : public CMDIChildWndEx
{
public:
	CChildFrame(SeasonSession* pSession);
	virtual ~CChildFrame();
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
private:
	CChildView m_wndView;
	SeasonSession* m_pSeasonSession;
	
protected:
	virtual void PostNcDestroy();
	
	
	
	afx_msg void OnFileClose();
	afx_msg void OnOpenRepository();
	afx_msg void OnCollector();
	afx_msg void OnSettings();

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnGetSeasonSession(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
