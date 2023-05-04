#include "stdafx.h"
#include "WelcomeDlg.h"
#include "../farmaccounts/FarmAccountsStorage.h"
#include "../farmaccounts/FarmAccountsManagerDlg.h"
#include "../ui/ExceptionMsgBox.h"
#include "../ctrls/stdedit/stdedit.h"
#include "../utils/Product.h"

BEGIN_MESSAGE_MAP(WelcomeDlg, CDialog)
	ON_BN_CLICKED(IDC_MANAGEFARMACCOUNTS,OnManageAccounts)
	ON_CBN_SELCHANGE(IDC_ACCOUNTSCOMBO,OnFarmAccountChange)
END_MESSAGE_MAP()


WelcomeDlg::WelcomeDlg(XContextInitialiser* initialiser) : CDialog(WelcomeDlg::IDD),m_initialiser(initialiser)
{
		
}

void WelcomeDlg::OnOK()
{
	int nSelected = m_accountsCombo.GetCurSel();
	if(nSelected < 0)
	{
		AfxMessageBox(IDS_ASKCHOOSEFARM);
		return;
	}

	uint32_t accountId = (uint32_t)m_accountsCombo.GetItemData(nSelected);
	FarmAccount* account = m_accounts.FindAccount(accountId);

	ExceptionMsgBox msgBox;
	try
	{
		m_initialiser->SetServer(account->GetAddress(),account->GetPort());
		m_initialiser->SetFarm(account->GetFarm());
		m_initialiser->SetLoginCredentials(account->GetLogin(),account->GetPassword());
		m_initialiser->Init();
		EndDialog(IDOK);
	}
	catch(wexception& e)
	{
		msgBox.Show(e);
	}

}


BOOL WelcomeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	FarmAccountsStorage storage;
	m_accounts = std::move(storage.Load());
		
	ReloadAccounts();

	if(m_accountsCombo.GetCount() == 1)
	{
		m_accountsCombo.SetCurSel(0);
		SetCredentials();
	}

	SetDlgItemText(IDC_VERSION, Product().Version().c_str());

	/*LANGID landId = GetThreadUILanguage();
	LANGID ll = MAKELANGID(9, 1);
	LANG_POLISH*/
	return TRUE;
}

void WelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ACCOUNTSCOMBO,m_accountsCombo);
	DDX_StdText(pDX,IDC_LOGIN,m_login);
	DDX_StdText(pDX,IDC_PASSWORD,m_password);
	
}


void WelcomeDlg::ReloadAccounts()
{
	uint32_t selectedAccountId = GetSelectedAccountId();
	
	m_accountsCombo.ResetContent();

	PtrEnumerator<const FarmAccount> accountEnum = m_accounts.EnumAccounts();
	while(accountEnum.hasNext())
	{
		const FarmAccount* pAccount = *accountEnum;
		int item = m_accountsCombo.AddString(pAccount->GetName().c_str());
		m_accountsCombo.SetItemData(item,(DWORD_PTR)pAccount->GetId());
		accountEnum.advance();
	}
	

	SelectAccount(selectedAccountId);
}




void WelcomeDlg::SelectAccount(uint32_t accountId)
{
	for(int i = 0, count = m_accountsCombo.GetCount();i < count;i++)
	{
		if(accountId == (uint32_t)m_accountsCombo.GetItemData(i))
		{
			m_accountsCombo.SetCurSel(i);
			return;
		}
	}

}

uint32_t WelcomeDlg::GetSelectedAccountId()
{
	int nSelected = m_accountsCombo.GetCurSel();

	if(nSelected >= 0)
	{
		return (uint32_t)m_accountsCombo.GetItemData(nSelected);
	}

	return NULL_ID;
}

void WelcomeDlg::SetCredentials()
{
	uint32_t accountId = GetSelectedAccountId();

	if(accountId != NULL_ID)
	{	
		FarmAccount* account = m_accounts.FindAccount(accountId);
		m_login = account->GetLogin();
		m_password = account->GetPassword();
		UpdateData(FALSE);
	}
}

void WelcomeDlg::OnManageAccounts()
{
	FarmAccountsManagerDlg accountsMgrDlg(&m_accounts,this);
	accountsMgrDlg.DoModal();

	FarmAccountsStorage storage;
	storage.Save(m_accounts);
	
	ReloadAccounts();
}

void WelcomeDlg::OnFarmAccountChange()
{
	SetCredentials();
}