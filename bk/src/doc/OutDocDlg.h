#ifndef __OUTDOCDLG_H__
#define __OUTDOCDLG_H__

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
#include "../context/transaction/OutDocTransaction.h"
#include "ctrls/ItemCountStatic.h"
#include "../settings/DefaultValues.h"

class OutDocDlg : public CSessionAwareDialog 
{
public:
	OutDocDlg(OutDocTransactionPtr transaction,CWnd* pParent);
	virtual ~OutDocDlg();
	
	enum { IDD = IDD_OWNDOC };
protected:
	CDocItemListCtrl m_cowlist;

	CMotiveCombo	m_MotiveCombo;
	CDateEdit		m_DocDate;
	CDateEdit		m_TransportDate;
	CHourMinuteEdit m_loadStartTm;
	CHourMinuteEdit m_loadEndTm;
	
	std::wstring		m_plateNo;
	CExtrasMemo		m_extras;
	//CMenu			m_listMenu;
//	CGrpCombo	m_activeGrpCombo;
	//CGrpSummaryListCtrl m_GrpSummaryList;
	CBaseCombo		m_AgentCb;		
	OutDocTransactionPtr m_transaction;
	DocTransactionContent m_TransactionContent;
	CItemCountStatic m_itemCount;


	DefaultValues m_defaultValues;

	//bool m_dlgshown;
protected:

	
	//virtual void DeleteCow(int iElement);
	//virtual void EditCow(int pElement);

//	virtual bool CommitDoc() = 0;
//	virtual CString GetDocDlgCaption() = 0;
//	virtual void SetupBtns()  = 0;
//	virtual void ModifyListMenu(CMenu &listMenu);
//	virtual void ShowPass(int nItem) = 0;
//	virtual void FindPass() = 0;

//	virtual void RightClick(CPoint &ptCursor);

	virtual void DoDataExchange(CDataExchange* pDX);    	
	virtual BOOL OnInitDialog() ;	
	virtual void OnOK();
	void SaveDocDetails(DocDetails& details);
	//void UpdateCowCount();
//	void UpdateSummary();
	//int GetActiveGroup();
	
//	virtual void ChangeGrp4Cow(Cow *pCow,int grp) = 0;
//	void SetPlateNo(const CString& sPlateNo);


protected:

	afx_msg void OnAddCow();
	afx_msg void OnDeleteCow();
//	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
//	afx_msg void OnGetCows();
//	afx_msg void OnCancel();	
//	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);

//	afx_msg void OnMenuEditCow();
//	afx_msg void OnMenuShowPass();
//	afx_msg void OnClose();
	afx_msg void OnFindPassports();
//	afx_msg void OnGrpChange(UINT grpItemId);

	afx_msg void OnSetLoadTimeStart();
	afx_msg void OnSetLoadTimeEnd();
	afx_msg void OnWindowPosChanged(WINDOWPOS* windowpos);
	afx_msg LRESULT OnAfterDlgShow(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSetPlateNo();
	afx_msg void OnDestroy();

//	afx_msg void OnSetPlateNoFromAgent();
//	afx_msg void OnSetPlateNoFromHent();

	afx_msg void OnClearAgent();
	
	DECLARE_MESSAGE_MAP()
};




#endif
