#ifndef __COWSELECTDLG_H__
#define __COWSELECTDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "../SessionAwareDialog.h"
#include "../ctrls/contentlistctrl/ContentListCtrl.h"
#include "CowSelectInterface.h"
#include "../barcodescanner/ScannerEventListener.h"
#include "../barcodescanner/ScannerConnectionStatus.h"
#include "../ctrls/tipwnd/TimerTipWnd.h"
#include "../barcodescanner/ui/ScannerStatusIndicator.h"
#include "../ctrls/cownowildcardedit/CowNoWildcardEdit.h"

class CowSelectDlg : public CDialog, protected ScannerEventListener
{
	class PickContentListCtrl : public CContentListCtrl
	{
	public:
		PickContentListCtrl(CowSelectInterface *pInterface);
		virtual void SortBy(const SortAttributes& attributes);
	private:
		CowSelectInterface *m_pInterface;
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		DECLARE_MESSAGE_MAP()
	};


public:
	CowSelectDlg(CowSelectInterface *pInterface,CWnd* pParent = NULL);
	enum { IDD = IDD_COWSELECT };


	void SetHeader(const ListHeaderDescriptor& header);
protected:
	PickContentListCtrl m_CandidatesList;
	CTimerTipWnd m_Msg;
	CCowNoWildcardEdit m_CowNoWildcardEdit;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog() ;
	void UpdateCount();
	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();
	void UpdateBarcodeIndictor();
	void PickRows(const std::vector<int>& rows);
protected:
	CowSelectInterface *m_pInterface;
	CScannerStatusIndicator m_indicator;
	
	afx_msg void OnPickRow();
	afx_msg void OnUnpickRow();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nTimerID);
	afx_msg void OnEditCowNoComplete(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#endif