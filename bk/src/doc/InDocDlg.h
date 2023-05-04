#ifndef __INDOCDLG_H__
#define __INDOCDLG_H__

#include <afxwin.h>
#include "../resource.h"

#include "..\ctrls\MotiveCombo.h"
#include "..\ctrls\dateedit\dateedit.h"
#include "../ctrls/BaseCombo.h"
#include "../ctrls/hourminuteedit/HourMinuteEdit.h"
#include "../ctrls/ExtrasMemo.h"
#include "../ctrls/BaseCombo.h"
#include "../SessionAwareDialog.h"
#include "ctrls/DocItemListCtrl.h"
#include "../context/SeasonSession.h"
#include "../content/DocTransactionContent.h"
#include "ctrls/ItemCountStatic.h"
#include "ctrls\RecognizePassportsBtn.h"
#include "../passportrecognition/cache/PassportImageDiskCache.h"
#include "../ctrls/countrybutton/CountryButton.h"
#include "../settings/DefaultValues.h"
#include "../ctrls/pngbutton/PngButton.h"

class InDocDlg : public CSessionAwareDialog 
{
public:
	InDocDlg(InDocTransactionPtr transaction,CWnd* pParent);
	virtual ~InDocDlg();
	
	enum { IDD = IDD_OWNDOC };
protected:
	CDocItemListCtrl m_cowlist;

	CMotiveCombo	m_MotiveCombo;
	CDateEdit		m_DocDate;
	CDateEdit		m_TransportDate;
	CHourMinuteEdit m_loadStartTm;
	CHourMinuteEdit m_loadEndTm;
	CItemCountStatic m_itemCount;
	CRecognizePassportsBtn m_GetCowsBtn;
	
	std::wstring		m_plateNo;
	CExtrasMemo		m_extras;
	//CMenu			m_listMenu;
//	CGrpCombo	m_activeGrpCombo;
	//CGrpSummaryListCtrl m_GrpSummaryList;
	CBaseCombo		m_AgentCb;		
	InDocTransactionPtr m_transaction;
	DocTransactionContent m_TransactionContent;
	PassportImageDiskCache m_passImgCache;
	CPngButton m_ScanBtn;

	DefaultValues m_defaultValues;
	
protected:
	void ScanCow();
	void OnCellDblClick(int row,uint32_t colId,const CRect& rect);
	void EditCow(ItemId cowItem);
	void OnItemWeight(int row,const NullDecimal& decimal);
	void OnItemClass(int row,uint32_t classId);
//	virtual void ModifyListMenu(CMenu &listMenu);
//	virtual void ShowPass(int nItem) = 0;
//	virtual void FindPass() = 0;

	virtual void DoDataExchange(CDataExchange* pDX);    	
	virtual BOOL OnInitDialog() ;	
	virtual void OnOK();
	void SaveDocDetails(DocDetails& details);

	//	void UpdateSummary();
	//int GetActiveGroup();
	
//	virtual void ChangeGrp4Cow(Cow *pCow,int grp) = 0;

protected:
	afx_msg void OnAddCow();
	afx_msg void OnDeleteCow();
	afx_msg void OnScanCow();
	
	afx_msg void OnPassRecgntn();
	afx_msg void OnPassRecgntnSettings();
	afx_msg void OnPassRecgntnUpdate();

//	afx_msg void OnMenuEditCow();
//	afx_msg void OnMenuShowPass();
	afx_msg void OnFindPassports();
//	afx_msg void OnGrpChange(UINT grpItemId);

	afx_msg void OnSetLoadTimeStart();
	afx_msg void OnSetLoadTimeEnd();
	afx_msg void OnSetPlateNo();
	afx_msg void OnDestroy();
	afx_msg void OnClearAgent();
	
	DECLARE_MESSAGE_MAP()
public:
	
};




#endif
