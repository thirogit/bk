#ifndef __FARMACCOUNTSMANAGERDLG_H__
#define __FARMACCOUNTSMANAGERDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "Farms.h"

class FarmAccountsManagerDlg : public CDialog
{
public:
	FarmAccountsManagerDlg(FarmAccounts* accounts,CWnd* pParent);
	enum { IDD = IDD_FARMACCOUNTSMANAGER };
	
protected:
	FarmAccounts* m_accounts;


	CListBox m_accountsList;
	
protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;
	virtual void	OnOK();
	

	void ReloadAccountList();
protected:	
	
	afx_msg void OnAddFarm();
	afx_msg void OnEditFarm();
	afx_msg void OnDuplicateFarm();
	afx_msg void OnDeleteFarm();

	DECLARE_MESSAGE_MAP()
};

#endif