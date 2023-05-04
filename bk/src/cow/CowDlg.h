#ifndef __COWDLG_H__
#define __COWDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "../ctrls/maskededit/OXMaskedEdit.h"
#include "../ctrls/ExtrasMemo.h"
#include "../ctrls/dateedit/DateEdit.h"
#include "../ctrls/CowNoEdit.h"
#include "../ctrls/LongitudeEdit.h"
#include "../ctrls/LatitudeEdit.h"
#include "../ctrls/decimaledit/DecimalEdit.h"
#include "../context/transaction/CowEditor.h"
#include "../ctrls/BaseCombo.h"
#include "../ctrls/cowsexbtn/CowSexBtn.h"
#include "../ctrls/hentbtn/HentBtn.h"
#include "../SessionAwareDialog.h"
#include "../barcodescanner/ui/ScannerStatusBtn.h"


class CowDlg : public CSessionAwareDialog
{
public:
	CowDlg(CowEditor *pEditor,CWnd* pParent = NULL);
	enum { IDD = IDD_COW };		
protected:
	CCowNoEdit 	  m_cowno;
	CDateEdit  	  m_birthdate;
	COXMaskedEdit m_passno;
	CDateEdit  	  m_passdate;
		
	CBaseCombo	  m_stock;
	CBaseCombo	  m_class;
	CExtrasMemo   m_extras;
	CCowNoEdit    m_motherean;
	CDecimalEdit  m_weight;
	CCowSexBtn	  m_sexbtns;
	CHentBtn	  m_hentbtn;

	std::wstring	  m_birthplace;
	
	//CTimerTipWnd  m_UsedPreDefinedSexTip;
	std::wstring	  m_HealthCertNo;
	CowNo		  m_initialCowNo;
	bool		  m_ignoreChkSum;
	
	CScannerStatusBtn m_ScanCowNoBtn;

protected:
	CowEditor *m_pEditor;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog() ;
	void ConfigureControls();
	void InitControls();
	bool Validate();
	void SaveCow();
	virtual void OnOK();
	void UpdateScannerStatusBtn();
protected:
	afx_msg void OnCowClassChange();
	afx_msg void OnClassComboLostFocus();
	afx_msg void OnFirstOwner();
	afx_msg void OnScanCowNo();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

#endif