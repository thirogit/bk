#ifndef __MOVEDOCDLG_H__
#define __MOVEDOCDLG_H__

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

class MoveDocDlg : public CSessionAwareDialog 
{
public:
	MoveDocDlg(MoveDocTransactionPtr transaction,CWnd* pParent);
	virtual ~MoveDocDlg();	
	enum { IDD = IDD_MOVEDOC };
protected:
	CDocItemListCtrl m_cowlist;
	
	CDateEdit		m_DocDate;
	CDateEdit		m_TransportDate;
	CHourMinuteEdit m_loadStartTm;
	CHourMinuteEdit m_loadEndTm;
	
	std::wstring		m_plateNo;
	CExtrasMemo		m_extras;
	CBaseCombo		m_AgentCb;		
	MoveDocTransactionPtr m_transaction;
	DocTransactionContent m_TransactionContent;

	CItemCountStatic m_itemCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    	
	virtual BOOL OnInitDialog() ;	
	virtual void OnOK();
	void SaveDocDetails(DocDetails& details);

	void OnCellDblClick(int row,uint32_t colId,const CRect& rect);
	void OnItemWeight(int row,const NullDecimal& decimal);


protected:
	afx_msg void OnAddCow();
	afx_msg void OnDeleteCow();
	afx_msg void OnSetLoadTimeStart();
	afx_msg void OnSetLoadTimeEnd();
	afx_msg void OnSetPlateNo();
	afx_msg void OnDestroy();
	afx_msg void OnFindPassports();
	afx_msg void OnClearAgent();

	DECLARE_MESSAGE_MAP()
};




#endif
