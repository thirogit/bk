#ifndef __HENTSEARCHDLG_H__
#define __HENTSEARCHDLG_H__

#include <afxwin.h>
#include "../resource.h"

#include "../ctrls/enteredit/EnterEdit.h"
#include "../ctrls/contentlistctrl\ContentListCtrl.h"
#include "../content/HentSearchContent.h"
#include "../ctrls/contentlistctrl/ForwardContentObserver.h"
#include "../SessionAwareDialog.h"
#include "../header/HeaderLayoutConfiguration.h"
#include "../ctrls/tipwnd/TimerTipWnd.h"
#include "../barcodescanner/ui/ScannerStatusIndicator.h"
#include "../barcodescanner/ScannerEventListener.h"
#include "../barcodescanner/ScannerConnectionStatus.h"

class HentSearchDlg : public CSessionAwareDialog, protected ForwardContentObserver, protected ScannerEventListener
{
	enum { IDD = IDD_HENTSEARCH };
public:
	HentSearchDlg(CWnd* pParent);	
	uint32_t FindHent();
protected:
	CContentListCtrl m_ResultList;
	CEnterEdit m_QueryPatternEdit;
	HentSearchContent m_ResultContent;
	uint32_t m_SelectedHentId;
	HeaderLayoutConfiguration m_hdrLayout;
	CTimerTipWnd m_Msg;
	CScannerStatusIndicator m_ScannerStatusInd;
	
protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;

	void QueryHent(const std::wstring& sQuery);
	void UpdateResultCount();
	void OnResultRowDblClick(int row);
	virtual void OnCountChanged();
	virtual void OnOK();
	uint32_t GetSelectedHent() const;
	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();
	void UpdateBarcodeIndictor();


	CDialog::DoModal;
	
protected:	
	afx_msg void OnQueryComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnQueryChange();
	afx_msg void OnSearchClicked();
	afx_msg void OnAddHent();
	afx_msg void OnClearResults();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

#endif