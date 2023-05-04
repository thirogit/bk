#ifndef __COMPANYINFODLG_H__
#define __COMPANYINFODLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "../SessionAwareDialog.h"
#include "../ctrls/contentlistctrl/ContentListCtrl.h"

class CompanyInfoDlg : public CSessionAwareDialog
{
	enum { IDD = IDD_COMPANYINFO };
public:
	CompanyInfoDlg(CWnd* pParent);	
	
protected:
		
protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;
	virtual void    OnOK();
	
protected:	

	DECLARE_MESSAGE_MAP()
};

#endif