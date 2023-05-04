#include "stdafx.h"
#include "HentDlg.h"
#include <data\datalimits.h>
#include "../utils/StringLoader.h"
#include <data\types\countries\Countries.h>
#include "../ui/waitdlg\WaitDialog.h"
#include "../context/ContextException.h"
#include "../AppServices.h"
#include "../cow/ScanCowBCdsWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(HentDlg, CSessionAwareDialog)

	ON_EN_SETFOCUS(IDC_ALIAS, OnAliasFocus)
	ON_EN_KILLFOCUS(IDC_ALIAS, OnAliasKillFocus)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SCANHENTNO,OnScanHentNo)

END_MESSAGE_MAP()


HentDlg::HentDlg(HentEditor* pEditor,CWnd* pParent) : CSessionAwareDialog(HentDlg::IDD,pParent), 
							   m_regon(L"[0-9]{7}([0-9]{2})?([0-9]{5})?"),
							   m_idno(L"^[a-zA-Z]{3}[0-9]{6}|^[a-zA-Z]{2}[0-9]{7}"),							   
							   m_pEditor(pEditor),
							   m_extras(MAXEXTRAS)
{
	
}
void HentDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	
	DDX_Control (pDX, IDC_ZIP, m_ZipCode) ;
	DDX_Control (pDX, IDC_NIP, m_FiscalNo) ;
	DDX_Control(pDX,IDC_PESEL,m_pesel);
	DDX_Control (pDX, IDC_WETNO, m_WetNo) ;
	DDX_Control (pDX, IDC_EXTRAS, m_extras) ;
	
	DDX_Control(pDX,IDC_REGON,m_regon);
	DDX_Control(pDX,IDC_IDNUMBER,m_idno);
	DDX_Control(pDX,IDC_ISSUEDATE,m_issuedate);	
	DDX_Control(pDX,IDC_ACCOUNTNO,m_accountno);
	DDX_Control(pDX,IDC_LONGITUDE,m_Longitude);
	DDX_Control(pDX,IDC_LATITUDE,m_Latitude);
	DDX_Control(pDX,IDC_HTYPE,m_henttype);

	DDX_StdText(pDX,IDC_ALIAS,m_alias);
	DDX_StdText(pDX,IDC_PHONE,m_phone);
	DDX_StdText(pDX,IDC_CITY,m_city);
	DDX_StdText(pDX,IDC_STREET,m_street);
	DDX_StdText(pDX,IDC_POBOX,m_pobox);
	DDX_StdText(pDX,IDC_ISSUEPOST,m_issuepost);
	DDX_StdText(pDX,IDC_BANKNAME,m_bankname);
	DDX_StdText(pDX,IDC_WETLICENCENO,m_wetlicenceno);
	DDX_StdText(pDX,IDC_CELLPHONENO,m_cellphoneno);
	DDX_StdText(pDX,IDC_EMAILADDRESS,m_emailaddress);
	DDX_StdText(pDX,IDC_PLATE,m_plate);
	DDX_StdText(pDX,IDC_NAME,m_name);
	DDX_Control (pDX, IDC_HENTNO, m_hentno) ;

	DDX_Control(pDX, IDC_SCANHENTNO, m_ScanHentNoBtn);
}

BOOL  HentDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();
	SetWindowText(GetWindowCaption().c_str());
	SetDlgItemText(IDOK,GetOkCaption().c_str());
	ConfigureDialogControls();
	InitDialogControls();
	return TRUE ;
}

std::wstring HentDlg::GetOkCaption()
{
	if(m_pEditor->IsEditing())
		return StringLoader(IDS_BTNEDIT);
	else
		return StringLoader(IDS_BTNADD);
}

std::wstring HentDlg::GetWindowCaption()
{
	if(m_pEditor->IsEditing())	
		return StringLoader(IDS_HENTCAPTIONEDIT);
	else
		return StringLoader(IDS_HENTCAPTIONNEW);
}

bool HentDlg::OkHent()
{
	SaveHent();
	CWaitDialog waitForSave(this,StringLoader(IDS_SAVINGHENT));
	try
	{
		waitForSave.WaitForTask([this]() -> void
								{
									m_pEditor->Save();
								});
		return true;
	}
	catch(ContextException& e)
	{
		AfxMessageBox(e.wwhat());
	}
	return false;
}

void HentDlg::InitDialogControls()
{
	m_ZipCode.SetText(m_pEditor->GetZip());
	m_FiscalNo.SetText(m_pEditor->GetFiscalNo());
	m_pesel.SetText(m_pEditor->GetPESEL());
	m_WetNo.SetText(m_pEditor->GetWetNo());	
	m_extras.SetText(m_pEditor->GetExtras());	
	m_regon.SetText(m_pEditor->GetREGON());
	m_idno.SetText(m_pEditor->GetIdNo());
	m_issuedate.SetDate(m_pEditor->GetIssueDate());
	m_accountno.SetAccountNo(m_pEditor->GetAccountNo());
	m_Longitude.SetLongitude(m_pEditor->GetLongitude());
	m_Latitude.SetLatitude(m_pEditor->GetLatitude());
	m_henttype.ReloadCombo(m_pEditor->GetHentType());
	m_alias = m_pEditor->GetAlias();
	m_phone = m_pEditor->GetPhoneNo();
	m_city = m_pEditor->GetCity();
	m_street = m_pEditor->GetStreet();
	m_pobox = m_pEditor->GetPOBox();
	m_issuepost = m_pEditor->GetIssuePost();
	m_bankname = m_pEditor->GetBankName();
	m_wetlicenceno = m_pEditor->GetWetLicenceNo();
	m_cellphoneno = m_pEditor->GetCellPhoneNo();
	m_emailaddress = m_pEditor->GetEmailAddress();
	m_plate = m_pEditor->GetPlateNo();
	m_name = m_pEditor->GetName();
	m_hentno.SetHentNo(m_pEditor->GetHentNo());
	m_accountno.SetAccountNo(m_pEditor->GetAccountNo());
	UpdateData(FALSE);
}


void HentDlg::ConfigureDialogControls()
{	
	m_ZipCode.SetMask(L"##-###") ;
	m_FiscalNo.SetMask(L"##########") ;
	m_WetNo.SetMask(L"########") ;	
	m_pesel.SetMask(L"###########");
	m_henttype.ReloadCombo();

	m_hentno.EnableButton();
	//m_hentno.SetCountry(Countries::GetCountryByNumber(616));
		
	CWnd* pCancelBtn = GetDlgItem(IDCANCEL);

	//m_HentNo.SetAbortWnd(pCancelBtn);	
	//m_accountno.SetAbortWnd(pCancelBtn);
	//m_Longitude.SetAbortWnd(pCancelBtn);
	//m_Latitude.SetAbortWnd(pCancelBtn);


	SendDlgItemMessage(IDC_NAME,EM_LIMITTEXT,MAXNAME);	
	SendDlgItemMessage(IDC_ALIAS,EM_LIMITTEXT,MAXALIAS);
	SendDlgItemMessage(IDC_PHONE,EM_LIMITTEXT,MAXPHONE);
	SendDlgItemMessage(IDC_CITY,EM_LIMITTEXT,MAXCITY);
	SendDlgItemMessage(IDC_STREET,EM_LIMITTEXT,MAXSTREET);
	SendDlgItemMessage(IDC_POBOX,EM_LIMITTEXT,MAXPOBOX);
	SendDlgItemMessage(IDC_ISSUEPOST,EM_LIMITTEXT,MAXISSUEPOST);
	SendDlgItemMessage(IDC_BANKNAME,EM_LIMITTEXT,MAXBANKNAME);
	SendDlgItemMessage(IDC_PLATE,EM_LIMITTEXT,MAXPLATE);
	SendDlgItemMessage(IDC_WETLICENCENO,EM_LIMITTEXT,MAXWETLICENCENO);
	SendDlgItemMessage(IDC_CELLPHONENO,EM_LIMITTEXT,MAXCELLPHONE);
	SendDlgItemMessage(IDC_EMAILADDRESS,EM_LIMITTEXT,MAXEMAILADDRESS);

	AppServices::GetBCScannerService()->AddListener(&m_ScanHentNoBtn);
	UpdateScannerStatusBtn();
	
}

void HentDlg::OnDestroy()
{
	AppServices::GetBCScannerService()->RemoveListener(&m_ScanHentNoBtn);
	CDialog::OnDestroy();
}

void HentDlg::UpdateScannerStatusBtn()
{
	m_ScanHentNoBtn.SetStatus(AppServices::GetBCScannerService()->GetConnectionStatus());
}

bool HentDlg::ValidateFields()
{
	UpdateData();

	if(m_hentno.IsInputEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYHENTNO);
		m_hentno.SetFocus();
		return false;
	}

	if(!m_hentno.IsFullPrompt()) 
	{
		AfxMessageBox(IDS_PARTIALHENTNO);
		m_hentno.SetFocus();
		return false;
	}
	
	if(m_name.empty())
    {
		AfxMessageBox(IDS_ERREMPTYNAME);
		return false;
	}

	if(m_alias.empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYALIAS);
		return false;
	}
	
	if(m_street.empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYSTREET);
		return false;
	}

	if(m_pobox.empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYPOBOX);
		return false;
	}

	if(m_city.empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYCITY);
		return false;
	}

	if(m_ZipCode.IsInputEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYZIP);
		m_ZipCode.SetFocus();
		return false;
	}	
		
	if(m_henttype.GetSelectedHentType() == HentType_None)
	{
		AfxMessageBox(IDS_ERRSELHENTTYPE);
		m_henttype.SetFocus();
		return false;
	}

	if(!m_regon.IsInputEmpty() && !m_regon.IsInputValid())
	{
		AfxMessageBox(IDS_ERRBADREGON);
		m_regon.SetFocus();
		return false;
	}
	if(!m_idno.IsInputEmpty() && !m_idno.IsInputValid())
	{
		AfxMessageBox(IDS_ERRBADIDNO);
		m_idno.SetFocus();
		return false;
	}

	if(!m_issuedate.IsInputEmpty() && !m_issuedate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADISSUEDATE);
		m_issuedate.SetFocus();
		return false;
	}

	return true;
}

void HentDlg::SaveHent()
{
	m_pEditor->SetZip(m_ZipCode.GetText());
	m_pEditor->SetFiscalNo(m_FiscalNo.GetInputData());
	m_pEditor->SetPESEL(m_pesel.GetInputData());
	m_pEditor->SetWetNo(m_WetNo.GetInputData());
	m_pEditor->SetExtras(m_extras.GetText());
	m_pEditor->SetREGON(m_regon.GetText());
	m_pEditor->SetIdNo(m_idno.GetText());
	m_pEditor->SetIssueDate(m_issuedate.GetDate());
	m_pEditor->SetAccountNo(m_accountno.GetAccountNo());
	m_pEditor->SetLongitude(m_Longitude.GetLongitude());
	m_pEditor->SetLatitude(m_Latitude.GetLatitude());
	m_pEditor->SetHentType(m_henttype.GetSelectedHentType());
	m_pEditor->SetAlias(m_alias);
	m_pEditor->SetPhoneNo(m_phone);
	m_pEditor->SetCity(m_city);
	m_pEditor->SetStreet(m_street);
	m_pEditor->SetPOBox(m_pobox);
	m_pEditor->SetIssuePost(m_issuepost);
	m_pEditor->SetBankName(m_bankname);
	m_pEditor->SetWetLicenceNo(m_wetlicenceno);
	m_pEditor->SetCellPhoneNo(m_cellphoneno);
	m_pEditor->SetEmailAddress(m_emailaddress);
	m_pEditor->SetPlateNo(m_plate);
	m_pEditor->SetName(m_name);
	m_pEditor->SetHentNo(m_hentno.GetHentNo());
}

void  HentDlg::OnOK()
{
	if(!ValidateFields()) 
		return;

	if(OkHent())
		EndDialog(IDOK); 
}

void HentDlg::OnAliasFocus()
{
	if(GetDlgItem(IDC_ALIAS)->GetWindowTextLength() == 0)
	{
		CString alias;
		GetDlgItemText(IDC_NAME,alias);
		alias.Remove(' ');
		alias.MakeUpper();
		SetDlgItemText(IDC_ALIAS,alias);
	}
}

void HentDlg::OnAliasKillFocus()
{

}


void HentDlg::OnScanHentNo()
{
	HentNoBarcodeInputDialog hentInputDlg(this);
	HentNo hentNo = hentInputDlg.InputHentNo();
	if (!hentNo.IsNull())
	{
		m_hentno.SetHentNo(hentNo);
	}

}
