#ifndef __FARMACCOUNTDLG_H__
#define __FARMACCOUNTDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "Farms.h"
#include "../ctrls/FilterEdit/FilterEdit.h"

class FarmAccountDlg : public CDialog
{
public:
	FarmAccountDlg(FarmAccount* account,CWnd* pParent);
	enum { IDD = IDD_FARMACCOUNT };
	
protected:
	FarmAccount* m_account;

	std::wstring m_accountName;
	CFilterEdit m_farmNo;

	std::wstring m_username;
	std::wstring m_password;

	std::wstring m_address;
	int m_port;

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;
	virtual void OnOK();
	

	void SetDlgItemFocus(UINT nID);
protected:	
	
	DECLARE_MESSAGE_MAP()
};

#endif