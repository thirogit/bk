#ifndef __EXCELEXPORTSETTINGSDLG_H__
#define __EXCELEXPORTSETTINGSDLG_H__

#include "../ctrls/optionchecklistbox/CheckOption.h"
#include <map>
#include "../ctrls/optionchecklistbox/OptionCheckListBox.h"
#include <arrays/NewPtrFlushArray.h>
#include "../datatypes/CowDataComparator.h"
#include "ExcelExportSettings.h"

class ExcelExportSettingsDlg : public CDialog
{

public:
	enum { IDD = IDD_EXCELEXPORTSETTINGS };

	ExcelExportSettingsDlg(ExcelExportSettings& settings,CWnd* pParent = NULL);   
	virtual ~ExcelExportSettingsDlg();
	bool Configure();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
    virtual void OnOK();		
	CDialog::DoModal;

	COptionCheckListBox m_checkList;
	BOOL m_bPutDocsInSeperateSheets;
	ExcelExportSettings& m_Settings;
	
	afx_msg void OnCheckChange();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	DECLARE_MESSAGE_MAP()
};

#endif