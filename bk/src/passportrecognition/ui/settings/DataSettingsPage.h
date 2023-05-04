#ifndef __DATASETTINGSPAGE_H__
#define __DATASETTINGSPAGE_H__

#include <afxdlgs.h>
#include "../../settings/DataSettings.h"
#include "../../../resource.h"
#include "../../../ctrls/BaseCombo.h"

class CDataSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDataSettingsPage)
public:
	CDataSettingsPage(DataSettings *pDataSettings);
	virtual ~CDataSettingsPage();

	enum { IDD = IDD_RECOGNITIONDATAOPTS };

protected:
	BOOL				m_bUppercaseHent;
	BOOL				m_bDoNotUpdateValueIfPresent;	
	CCheckListBox 		m_FieldSelection;	

	CBaseCombo			m_DefaultStockCb;
	BOOL				m_bUseDefaultStock;

	DataSettings*		m_pDataSettings;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldOnOff();
	afx_msg void OnUseDefaultStockClicked();

	DECLARE_MESSAGE_MAP()

};

#endif