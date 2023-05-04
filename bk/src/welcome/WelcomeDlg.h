#ifndef __WELCOMEDLG_H__
#define __WELCOMEDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "../context/ContextInitialiser.h"
#include "../farmaccounts/Farms.h"


class WelcomeDlg : public  CDialog 
{
public:
	WelcomeDlg (XContextInitialiser* initialiser);

	enum { IDD = IDD_WELCOMEDLG };
	
protected:
	XContextInitialiser* m_initialiser;
	FarmAccounts m_accounts;
	
	std::wstring m_login;
	std::wstring m_password;
	CComboBox m_accountsCombo;	
	    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();

	uint32_t GetSelectedAccountId();
	void ReloadAccounts();
	void SetCredentials();
	void SelectAccount(uint32_t selectedAccountId);
	
	void OnOK();	
	

protected:
		
	afx_msg void OnFarmAccountChange();
	afx_msg void OnManageAccounts();
	DECLARE_MESSAGE_MAP()
	
};


#endif