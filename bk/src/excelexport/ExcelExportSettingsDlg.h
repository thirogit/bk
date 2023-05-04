#ifndef __EXCELEXPORTSETTINGSDLG_H__
#define __EXCELEXPORTSETTINGSDLG_H__

#include "ExcelExportSettings.h"
#include "../resource.h"
#include "../ctrls/layoutlistctrl/ListHeaderDescriptor.h"

class ExcelExportSettingsDlg : public CDialog
{
public:
	enum { IDD = IDD_EXCELEXPORTSETTINGS };

	ExcelExportSettingsDlg(ExcelExportSettings* settings,
						   const ListHeaderDescriptor* headerDescriptor,
						   CWnd* pParent = NULL);   
						   
	virtual ~ExcelExportSettingsDlg();
	bool Configure();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
    virtual void OnOK();		
	CDialog::DoModal;

	void MoveItemDown(int nItem);
	void MoveItemUp(int nItem);
	void MoveItem(int nSrcItemPos, int nDstItemPos);
	std::wstring GetItemText(CListBox& list, int item);

protected:
	CListBox m_LeftList;
	CListBox m_RightList;
	ExcelExportSettings* m_Settings;
	const ListHeaderDescriptor* m_HeaderDescriptor;

private:
	afx_msg void OnMoveToLeft();
	afx_msg void OnMoveToRight();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	DECLARE_MESSAGE_MAP()
};

#endif