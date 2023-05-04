#ifndef __REPODLG_H__
#define __REPODLG_H__

#include "../RepoSession.h"
#include "../../SessionAwareDialog.h"
#include "../../resource.h"
#include "../../ui/lazydlg/LazyDlg.h"

#include "content\PurchaseHeaderListContent.h"
#include "ctrls/RepoPurchaseListCtrl.h"
#include "RepoPurchaseContentPage.h"
#include "RepoPurchaseHentsPage.h"
#include "RepoPurchaseSummaryPage.h"
#include "../../ctrls/XTabCtrl/XTabCtrl.h"
#include "../../ctrls/pngmenubutton/PngMenuButton.h"
#include "../../ctrls/pngbutton/PngButton.h"
#include "../../ctrls/tipwnd/TimerTipWnd.h"


class CRepoDlg : public CLazyDlg<CSessionAwareDialog>
{
	DECLARE_DYNAMIC(CRepoDlg)

public:
	CRepoDlg(CWnd* pParent = NULL);
	virtual ~CRepoDlg();	
	enum { IDD = IDD_REPO };

private:
	RepoSession* m_pRepoSession;
	DateTime m_ViewedMonth;
	CRepoPurchaseListCtrl m_PurchaseList;
	PurchaseHeaderListContent m_PurchaseListContent;
	std::map<std::wstring, Purchase*> m_PurchaseMap;

	CRepoPurchaseHentsPage m_PurchaseHentsPage;
	CRepoPurchaseContentPage m_PurchaseContentPage;
	CRepoPurchaseSummaryPage m_PurchaseSummaryPage;
	CXTabCtrl m_PurchaseTabs;
	
	std::wstring m_selectedPurchaseId;

	CPngMenuBtn m_DownloadPurchaseBtn;
	CPngButton m_RefreshBtn;
	CPngButton m_SwitchToNowMonth;
	CPngButton m_PrevMonth;
	CPngButton m_NextMonth;
	CTimerTipWnd m_Msg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnAfterDlgShow();
	virtual void OnCancel();
	void ConnectAndQuery();
	void Query();
	void WaitAndQuery();
	void WaitAndFetchPurchase(const std::wstring& purchaseId);
	void FetchPurchase(const std::wstring& purchaseId);
	void DoDisconnect();
	void DisableAllControls();
	void UpdateCurrentMonthBtnCaption();
	void OnPurchaseSelected(const std::wstring& purchaseId);
	void OnPurchaseUnselected(const std::wstring& purchaseId);
	void OnDownloadWithDate();
	void ShowPurchaseDownloadedMsg();

	void DownloadPurchase(DateTime dt);
	afx_msg void OnDownload();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnPrevMonth();
	afx_msg void OnNextMonth();
	afx_msg void OnSwitchToCurrentMonth();
	afx_msg void OnRefresh();
	afx_msg void OnPurchaseClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPurchaseListKeyDn(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton4();
};

#endif 