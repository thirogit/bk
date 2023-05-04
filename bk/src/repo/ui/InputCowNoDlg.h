#ifndef __INPUTCOWNODLG_H__
#define __INPUTCOWNODLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../ctrls/CowNoEdit.h"

class CInputCowNoDlg : public CDialog
{
public:
	CInputCowNoDlg(CWnd* pParent = NULL);
	virtual ~CInputCowNoDlg();
	enum { IDD = IDD_EDITBOXDLG };

	CowNo ask();

protected:
	CCowNoEdit  m_CowNoEdit;
	CDialog::DoModal;

	CowNo	m_CowNo;
	
	virtual BOOL OnInitDialog();
	void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
};

#endif