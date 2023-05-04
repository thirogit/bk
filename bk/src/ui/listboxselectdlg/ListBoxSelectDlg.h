#ifndef __LISTBOXSELECTDLG_H__
#define __LISTBOXOSELECTDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../ctrls/BaseCombo.h"

class CListBoxSelectDlg : public CDialog
{
public:
	CListBoxSelectDlg(CWnd* pParent = NULL);
	virtual ~CListBoxSelectDlg();
	uint32_t Select(ComboContent& content,uint32_t itemId);
	void SetCaption(const std::wstring& caption);
	
	enum { IDD = IDD_LISTBOXSELECT };	
protected:	
	CDialog::DoModal;

	CListBox m_list;

	ComboContent m_content;
	uint32_t m_defaultItem;
	std::wstring m_sCaption;
	uint32_t m_selectedItemId;
	
	virtual BOOL OnInitDialog();
	void OnOK();
	uint32_t GetSelectedItemId();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	DECLARE_MESSAGE_MAP()
};


#endif