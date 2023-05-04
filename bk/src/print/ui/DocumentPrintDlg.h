#ifndef __PRINTDLG_H__
#define __PRINTDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../SessionAwareDialog.h"
#include "../DocumentArchetype.h"
#include "PrintPreviewCtrl.h"
#include <progress\SimpleProgress.h>
#include "../../ctrls/xtoolbar/XToolBar.h"
#include "../../bkprint/renderer/PrintOut.h"

class DocumentPrintDlg : public CSessionAwareDialog
{
public:
	DocumentPrintDlg(DocumentInstance* instance,CWnd* pParent = NULL);
	enum { IDD = IDD_PRINT };
	
protected:
	CPreviewViewCtrl m_previewCtrl;
	CSliderCtrl m_pageSliderCtrl;
	XToolBar m_profilesToolbar;
	CListBox m_profilesList;
	
	bkprint::PrintOut* m_pPrintOut;
	DocumentInstance* m_instance;
	bool m_dlgshown;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	
	void WaitGeneratePrintOut();
	void GeneratePrintOut();
	void DoPrint(int nCopies);
	void PerformPrinting(CPrintInfo* pPrintInfo,SimpleProgress* progress);
	void UpdatePagesIndicator();
	IPrintProfile* GetSelectedPrintProfile();

	void OnProfileAdd();
	void OnProfileDelete();
	void OnProfileEdit();
	void InitProfilesList();
	void SelectProfile(uint32_t profileId);
	void ReloadProfilesList();
protected:
	
	afx_msg void OnWindowPosChanged(WINDOWPOS* windowpos);
	afx_msg LRESULT OnAfterDlgShow(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnDoPrint();
	afx_msg void OnDoPrint2();
	afx_msg void OnDoPrint3();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnProfileSelectChange();
	DECLARE_MESSAGE_MAP()
};

#endif