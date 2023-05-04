#ifndef __COMBOSELECTDLG_H__
#define __COMBOSELECTDLG_H__

#include "../../ctrls/BaseCombo.h"
#include "../../Resource.h"

class CComboSelectDlg : public CDialog
{
public:
	CComboSelectDlg(CWnd* pParent = NULL);   
	virtual ~CComboSelectDlg();
	uint32_t Select(ComboContent& content,uint32_t itemId);
	void SetCaption(const std::wstring& caption);
	
	enum { IDD = IDD_COMBOSELECT };	
protected:	
	CDialog::DoModal;

	CBaseCombo m_combo;

	ComboContent m_content;
	uint32_t m_defaultItem;
	std::wstring m_sCaption;
	uint32_t m_selectedItemId;
	
	virtual BOOL OnInitDialog();
	void OnOK();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	DECLARE_MESSAGE_MAP()
};


#endif