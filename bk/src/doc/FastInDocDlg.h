#ifndef __FASTINDOCDLG_H__
#define __FASTINDOCDLG_H__

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
#include "../ctrls/countrybutton/CountryButton.h"
#include "../settings/DefaultValues.h"
#include "../ctrls/pngbutton/PngButton.h"
#include "../barcodescanner/ScannerEventListener.h"
#include "../ctrls/CowNoEdit.h"
#include "../ctrls/decimaledit/DecimalEdit.h"
#include "../ctrls/cowsexbtn/CowSexBtn.h"
#include "../ctrls/tipwnd/TimerTipWnd.h"
#include "../barcodescanner/ui/ScannerStatusIndicator.h"

class FastInDocDlg : public CSessionAwareDialog, public ScannerEventListener
{
public:
	FastInDocDlg(InDocTransactionPtr transaction,CWnd* pParent);
	virtual ~FastInDocDlg();
	
	enum { IDD = IDD_FASTINDOC };
protected:
	CDocItemListCtrl m_cowlist;

	CMotiveCombo	m_MotiveCombo;
	CDateEdit		m_DocDate;
	CDateEdit		m_TransportDate;
	CHourMinuteEdit m_loadStartTm;
	CHourMinuteEdit m_loadEndTm;
	CItemCountStatic m_itemCount;
	
	std::wstring		m_plateNo;
	CExtrasMemo		m_extras;
	
	CBaseCombo		m_AgentCb;		
	
	InDocTransactionPtr m_transaction;
	DocTransactionContent m_TransactionContent;
	
	DefaultValues m_defaultValues;

	CScannerStatusIndicator m_indicator;
	CCowNoEdit 	  m_cowno;
	CCowSexBtn	  m_sexbtns;
	CDecimalEdit  m_weight;
	CBaseCombo	  m_class;
	CTimerTipWnd  m_Msg;
	

protected:
	void OnCellDblClick(int row,uint32_t colId,const CRect& rect);
	void EditCow(ItemId cowItem);
	void OnItemWeight(int row,const NullDecimal& decimal);
	void OnItemClass(int row,uint32_t classId);

	virtual void DoDataExchange(CDataExchange* pDX);    	
	virtual BOOL OnInitDialog() ;	
	virtual void OnOK();
	void SaveDocDetails(DocDetails& details);

	void AddCowWithNumber(const CowNo& cowNo);
	void ShowErrorTooltip(const std::wstring& text);


	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();	
	void UpdateBarcodeIndictor();


protected:
	afx_msg void OnDeleteCow();
	afx_msg void OnSetLoadTimeStart();
	afx_msg void OnSetLoadTimeEnd();
	afx_msg void OnSetPlateNo();
	afx_msg void OnDestroy();
	afx_msg void OnClearAgent();
	afx_msg void OnAddCow();
	
	DECLARE_MESSAGE_MAP()
public:
	
};




#endif
