#include "stdafx.h"
#include "FarmAccountDlg.h"
#include <data\datalimits.h>
#include <data\IIdable.h>
#include "../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(FarmAccountDlg, CDialog)
		
END_MESSAGE_MAP()

#define MAX_FARMACCOUNTNAME 40
#define MAX_LOGIN 40
#define MAX_PASSWORD 40
#define MAX_ADDRESS 64



FarmAccountDlg::FarmAccountDlg(FarmAccount* account,CWnd* pParent) : CDialog(FarmAccountDlg::IDD,pParent), m_account(account)
{	
}

void FarmAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_StdText (pDX, IDC_ACCOUNTNAME, m_accountName) ;
	DDX_StdText (pDX, IDC_HOST, m_address) ;

	DDX_StdText (pDX, IDC_LOGIN, m_username) ;
	DDX_StdText (pDX, IDC_PASSWORD, m_password) ;
	DDX_Text (pDX, IDC_PORT, m_port) ;
	
	DDX_Control (pDX, IDC_FARMNO, m_farmNo);
	
}


BOOL FarmAccountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(m_account->GetId() != NULL_ID)
		SetWindowText(StringLoader(IDS_EDITGFARMACCOUNT).c_str());
	else
		SetWindowText(StringLoader(IDS_NEWFARMACCOUNT).c_str());

	SendDlgItemMessage(IDC_ACCOUNTNAME,EM_LIMITTEXT,MAX_FARMACCOUNTNAME);	
	SendDlgItemMessage(IDC_HOST,EM_LIMITTEXT,MAX_ADDRESS);
	SendDlgItemMessage(IDC_LOGIN,EM_LIMITTEXT,MAX_LOGIN);
	SendDlgItemMessage(IDC_PASSWORD,EM_LIMITTEXT,MAX_PASSWORD);
	m_farmNo.SetRegEx(L"[A-Za-z]{2}[0-9]{5,9}");
	
	m_accountName = m_account->GetName();
	m_address = m_account->GetAddress();
	m_username = m_account->GetLogin();
	m_password = m_account->GetPassword();

	m_port = m_account->GetPort();
		
	m_farmNo.SetText(m_account->GetFarm().ToString());

	UpdateData(FALSE);
			
	return TRUE ;
}


void FarmAccountDlg::OnOK()
{

	UpdateData();

	if(m_accountName.empty())
	{
		AfxMessageBox(IDS_ERREMPTYFARMNAME);
		SetDlgItemFocus(IDC_ACCOUNTNAME);
		return;
	}

	if(m_address.empty())
	{
		AfxMessageBox(IDS_ERREMPTYADDRESS);
		SetDlgItemFocus(IDC_ACCOUNTNAME);
		return;
	}
	
	if(m_username.empty())
	{
		AfxMessageBox(IDS_ERREMPTYLOGIN);
		SetDlgItemFocus(IDC_ACCOUNTNAME);
		return;
	}

	try
	{
		FarmNo farmNo = FarmNoFactory::FromString(m_farmNo.GetText());
		m_account->SetFarm(farmNo);
	}
	catch(...)
	{
		AfxMessageBox(IDS_ERRNVALIDFARMNO);
		SetDlgItemFocus(IDC_FARMNO);
		return;
	}

	m_account->SetAddress(m_address);
	m_account->SetName(m_accountName);
	m_account->SetPassword(m_password);
	m_account->SetLogin(m_username);	
	m_account->SetPort(m_port);
	

	EndDialog(IDOK);
}

void FarmAccountDlg::SetDlgItemFocus(UINT nID)
{
	CWnd* pDlgItem = GetDlgItem(nID);
	if(pDlgItem)
		pDlgItem->SetFocus();
}


