#ifndef __FINDCONTAINERDLG_H__
#define __FINDCONTAINERDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../ctrls/xcolorstatic/XColorStatic.h"
#include "../../ctrls/xtoolbar/XToolBar.h"
#include "../../ctrls/arrowbutton/ArrowButton.h"
#include <types\NullInt.h>
#include "../../SessionAwareDialog.h"
#include "../../ctrls/pngbutton/PngButton.h"

#define WM_FIND (WM_APP+1001)
#define WM_HIDEFIND (WM_APP+1004)
#define WM_CLEARFOUND (WM_APP+1002)
#define WM_SORTFOUND (WM_APP+1003)

class FindDlg;

class FindContainerDlg : public CSessionAwareDialog
{
public:
	FindContainerDlg(CRuntimeClass* pContentClass,CWnd* pParent);
	enum { IDD = IDD_FINDCONTAINER };
	void Create();
protected:
	CPngButton m_HideBtn;
	CXColorStatic m_FoundLabel;
	CXColorStatic m_FoundCount;
	XToolBar m_Toolbar;
	FindDlg* m_pContent;
	CRuntimeClass* m_pContentClass;
	CSize m_InitialSize;
	CFont m_foundCountFont;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	void UpdateFoundCount(const NullInt& count);
	void OnClearFound();
	void OnAppend();
	
protected:
	

	afx_msg void OnFindRequested(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHide();
	afx_msg void OnSize(UINT nType, int cx, int cy);

		
	DECLARE_MESSAGE_MAP()
};

#endif