#include "stdafx.h"
#include "FarmAccountsManagerDlg.h"
#include "FarmAccountDlg.h"

#define DEFAULT_PORT 50001

BEGIN_MESSAGE_MAP(FarmAccountsManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDFARM,OnAddFarm)		
	ON_BN_CLICKED(IDC_EDITFARM,OnEditFarm)		
	ON_BN_CLICKED(IDC_DUPLICATEFARM,OnDuplicateFarm)		
	ON_BN_CLICKED(IDC_DELETEFARM,OnDeleteFarm)		
END_MESSAGE_MAP()

FarmAccountsManagerDlg::FarmAccountsManagerDlg(FarmAccounts* accounts,CWnd* pParent) : CDialog(FarmAccountsManagerDlg::IDD,pParent),m_accounts(accounts)						  
{	
}

void FarmAccountsManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX,IDC_ACCOUNTSLIST,m_accountsList);
	
}


BOOL FarmAccountsManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ReloadAccountList();
			
	return TRUE ;
}


void FarmAccountsManagerDlg::ReloadAccountList()
{
	m_accountsList.ResetContent();

	int item;
	const FarmAccount* account;
	PtrEnumerator<const FarmAccount> accountEnum = m_accounts->EnumAccounts();
	while(accountEnum.hasNext())
	{
		account = *accountEnum;
		item = m_accountsList.AddString(account->GetName().c_str());
		m_accountsList.SetItemData(item,(DWORD_PTR)account->GetId());
		accountEnum.advance();
	}

}

void FarmAccountsManagerDlg::OnAddFarm()
{
	FarmAccount* newAccount = new FarmAccount(0);
	newAccount->SetPort(DEFAULT_PORT);
	FarmAccountDlg farmAccountdlg(newAccount,this);
	if(farmAccountdlg.DoModal() == IDOK)
	{
		m_accounts->AddAccount(*newAccount);
	}
	delete newAccount;
	ReloadAccountList();
}

void FarmAccountsManagerDlg::OnEditFarm()
{
	int nSelected = m_accountsList.GetCurSel();
	if(nSelected >= 0)
	{
		uint32_t accountId = m_accountsList.GetItemData(nSelected);
		FarmAccount* account = m_accounts->FindAccount(accountId);
	
		FarmAccountDlg farmAccountdlg(account,this);
		if(farmAccountdlg.DoModal() == IDOK)
		{
			ReloadAccountList();
		}		
	}
}

void FarmAccountsManagerDlg::OnDuplicateFarm()
{
	int nSelected = m_accountsList.GetCurSel();
	if(nSelected >= 0)
	{
		uint32_t accountId = m_accountsList.GetItemData(nSelected);
		FarmAccount* account = m_accounts->FindAccount(accountId);
		m_accounts->AddAccount(*account);
		ReloadAccountList();
	}
}

void FarmAccountsManagerDlg::OnDeleteFarm()
{
	int nSelected = m_accountsList.GetCurSel();
	if(nSelected >= 0)
	{
		uint32_t accountId = m_accountsList.GetItemData(nSelected);
		m_accounts->DeleteAccount(accountId);
		ReloadAccountList();
	}
}


void FarmAccountsManagerDlg::OnOK()
{
	EndDialog(IDOK);
}