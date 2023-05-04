#ifndef __PRINTPROFILEDLG_H__
#define __PRINTPROFILEDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../options/DocumentCfgDescription.h"
#include <configuration\print\PrintProfile.h>
#include "PrintProfileEditor.h"
#include "verticalstackctrl\VerticalStackCtrl.h"
#include "optionctrls\DocumentOptionCtrl.h"
#include "../../ctrls/stdedit/stdedit.h"
#include "PrintRecordOrderDlg.h"

class PrintProfileDlg : public CDialog
{
public:
	PrintProfileDlg(const DocumentCfgDescription* configuration, PrintProfileEditor* pEditor, CWnd* pParent = NULL);
	enum { IDD = IDD_PRINTPROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	CRect GetTabRect();
	void AddTab(CWnd* tabWnd, const std::wstring& sTabText);
	
	std::vector<DocumentOptionCtrl*> m_optionCtrls;
	VertialStackCtrl m_verticalStackWnd;
	PrintRecordOrderDlg m_sortingOrderWnd;

	const DocumentCfgDescription* m_configuration;
	PrintProfileEditor* m_pEditor;
	CStdEdit m_profileName;

	CTabCtrl m_naviTab;
	int m_prevSelectedTab;

protected:
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif