#ifndef __FINDCOWDLG_H__
#define __FINDCOWDLG_H__


#include "FindDlg.h"
#include "../../resource.h"
#include "../../ctrls/FilterEdit/FilterEdit.h"
#include "../../ctrls/daterange/DateRangeCtrl.h"
#include "../../ctrls/decimalrange/DecimalRangeCtrl.h"
#include "../../ctrls/agerange/AgeRangeCtrl.h"
#include "../../barcodescanner/ui/ScannerStatusIndicator.h"
#include "../../ctrls/cowsexcombo/CowSexCombo.h"
#include "../../ctrls/BaseCombo.h"
#include "../../ctrls/pngbutton/PngButton.h"
#include "../../ctrls/hentbtn/HentBtn.h"
#include "../../barcodescanner/ScannerEventListener.h"

class FindCowDlg : public FindDlg, private ScannerEventListener
{
	DECLARE_DYNCREATE(FindCowDlg)
public:
	FindCowDlg();
	enum { IDD = IDD_FINDHERDCOW };	
	virtual void Create(FindContainerDlg* container);
protected:
	FindContainerDlg* m_container;
	CFilterEdit m_CowNo;
	CFilterEdit m_PassportNo;
	CDateRangeCtrl m_BirthDtSpan;
	BOOL m_ByBirthDt;
	CScannerStatusIndicator m_scannerIndicator;
	CPngButton			m_clearStockBtn;
	CPngButton			m_clearClassBtn;
	CPngButton			m_clearSexBtn;

	CDecimalRangeCtrl m_WeightSpan;
	CAgeRangeCtrl m_AgeSpan;
	CBaseCombo m_stocksCB;
	CBaseCombo m_classesCB;	
	CCowSexCombo m_cowSexCB;

	CHentBtn m_BoughtFrom;
	BOOL m_ByBoughtFromEnabled;
	CHentBtn m_SoldTo;
	BOOL m_BySoldToEnabled;

	CDateRangeCtrl m_EntryDtSpan;
	BOOL m_ByEntryDtEnabled;
	CDateRangeCtrl m_ExitDtSpan;
	BOOL m_ByExitDtEnabled;



protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog() ;
	void OnOK();
	void OnCancel();
	void OnHent(CHentBtn& btn);
	

	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();
protected:
	
	afx_msg void OnHerdEntryDtEnable();
	afx_msg void OnHerdExitDtEnable();
	afx_msg void OnBoughtFromEnable();
	afx_msg void OnSoldToEnable();
	afx_msg void OnBirthDtEnable();
	afx_msg LRESULT OnFind(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClearStock();
	afx_msg void OnClearClass();
	afx_msg void OnClearCowSex();

	afx_msg void OnBoughtFromHent();
	afx_msg void OnSoldToHent();

	afx_msg void OnByCowNoKillFocus();
	afx_msg void OnByCowNoSetFocus();
	afx_msg void OnDestroy();
		
	DECLARE_MESSAGE_MAP()

};

#endif