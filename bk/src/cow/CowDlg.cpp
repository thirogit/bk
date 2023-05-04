#include "stdafx.h"
#include "CowDlg.h"
#include <data\datalimits.h>
#include "../utils/StringLoader.h"
#include "../hent/HentSearchDlg.h"
#include "../resource.h"
#include <validation\CowNoValidator.h>
#include <data\types\countries\Countries.h>
#include "../content/ClassesComboContent.h"
#include "../content/StocksComboContent.h"
#include "../AppServices.h"
#include "../cow/ScanCowBCdsWizard.h"

//#define PREDEFSEX_SETTING_NOTIFY_DELAY 1600

BEGIN_MESSAGE_MAP(CowDlg, CSessionAwareDialog)
			ON_CBN_SELCHANGE(IDC_COMBOSTOCK, OnCowClassChange)
			ON_CBN_KILLFOCUS(IDC_COMBOSTOCK,OnClassComboLostFocus)
			ON_BN_CLICKED(IDC_FIRSTOWNER,OnFirstOwner)
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_SCANCOWNO, OnScanCowNo)

END_MESSAGE_MAP()


CowDlg::CowDlg(CowEditor *pEditor,CWnd *pParent/* = NULL*/) : 
	CSessionAwareDialog(CowDlg::IDD,pParent),m_extras(MAXEXTRAS),m_pEditor(pEditor),m_weight(3)
{
}

void CowDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	DDX_Control (pDX, IDC_COWNO  , m_cowno  ) ;
	DDX_Control (pDX, IDC_BIRTHDATE, m_birthdate ) ;
	DDX_Control (pDX, IDC_PASSNO, m_passno  ) ;	
	DDX_Control (pDX, IDC_STOCKCOMBO,m_stock);
	DDX_Control (pDX, IDC_CLASSCOMBO,m_class);
	DDX_Control (pDX, IDC_PASSDATE,m_passdate);
	DDX_Control (pDX, IDC_EXTRAS,m_extras);
	DDX_Control (pDX, IDC_MOTHEREAN, m_motherean);
	DDX_Control (pDX, IDC_WEIGHT, m_weight);
	DDX_StdText	(pDX, IDC_BIRTHPLACE, m_birthplace);	
	DDX_StdText (pDX, IDC_HEALTHCERTNO,m_HealthCertNo);
	DDX_Control (pDX, IDC_SEXBTNS,m_sexbtns);
	DDX_Control (pDX, IDC_FIRSTOWNER,m_hentbtn);
	DDX_Control(pDX, IDC_SCANCOWNO, m_ScanCowNoBtn);

}
BOOL CowDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();	
	m_ignoreChkSum = false;

	std::wstring sCaption = m_pEditor->IsEditing() ? StringLoader(IDS_COWCAPTIONEDIT) : StringLoader(IDS_COWCAPTIONNEW);
	SetWindowText(sCaption.c_str());	

	ConfigureControls();
	InitControls();

		
	//m_ean.SetAbortWnd(&m_CowCancel);
	//m_motherean.SetAbortWnd(&m_CowCancel);
	//EnableToolTips(TRUE);
		
	//m_UsedPreDefinedSexTip.Create(this);
	
	AppServices::GetBCScannerService()->AddListener(&m_ScanCowNoBtn);
	UpdateScannerStatusBtn();

	return TRUE ;
}


void CowDlg::OnDestroy()
{
	AppServices::GetBCScannerService()->RemoveListener(&m_ScanCowNoBtn);
	CDialog::OnDestroy();
}

void CowDlg::UpdateScannerStatusBtn()
{
	m_ScanCowNoBtn.SetStatus(AppServices::GetBCScannerService()->GetConnectionStatus());
}

void CowDlg::ConfigureControls()
{
	m_passno.SetMask(L">>########") ;
	SendDlgItemMessage(IDC_BIRTHPLACE,EM_LIMITTEXT,MAXBIRTHPLACE);
	SendDlgItemMessage(IDC_HEALTHCERTNO,EM_LIMITTEXT,MAXHEALTHCERTNO);
	
	SeasonSession* pSession = GetSeasonSession();
	StocksComboContent stocksContent;
	ClassesComboContent classContent;

	stocksContent.CreateContent(pSession);							
	classContent.CreateContent(pSession);

	m_class.SetContent(classContent);
	m_stock.SetContent(stocksContent);
	m_cowno.EnableButton();
	m_motherean.EnableButton();
}

void CowDlg::InitControls()
{
	Country defaultCountry = Countries::GetCountryByNumber(616);

	m_stock.Select(m_pEditor->GetStockId());
	m_class.Select(m_pEditor->GetClassId());

	CowNo cowNo = m_pEditor->GetCowNo();
	if (cowNo.IsNull())
	{			
		m_cowno.SetCountry(defaultCountry);
	}
	else
	{
		m_cowno.SetCowNo(cowNo);
	}

	m_birthdate.SetDate(m_pEditor->GetBirthDate());
	m_passno.SetText(m_pEditor->GetPassNo());
	m_passdate.SetDate(m_pEditor->GetPassDate());
	m_extras.SetText(m_pEditor->GetExtras());


	CowNo motherNo = m_pEditor->GetMotherNo();
	if (motherNo.IsNull())
	{
		m_motherean.SetCountry(defaultCountry);
	}
	else
	{
		m_motherean.SetCowNo(motherNo);
	}

	Decimal weight = m_pEditor->GetWeight();
	if (weight > Decimal(0L))
	{
		m_weight.SetDecimal(weight);
	}
	
	m_birthplace = m_pEditor->GetBirthPlace();
	m_HealthCertNo = m_pEditor->GetHealthCertNo();
	m_sexbtns.SetSex(m_pEditor->GetSex());
	m_hentbtn.SetHent(m_pEditor->GetFirstOwnerId());
	m_initialCowNo = m_pEditor->GetCowNo();
	UpdateData(FALSE);
	
		
	
	

	
}

bool CowDlg::Validate()
{
	UpdateData();
	
	CowNo cowNo = m_cowno.GetCowNo();

	if(cowNo.IsNull()) 
	{
		AfxMessageBox(IDS_ERREMPTYCOWNO,MB_OK | MB_ICONERROR,0);
		m_cowno.SetFocus();
		return false;
	}

	if(!CowNoValidator::IsValid(cowNo))
	{
		if(AfxMessageBox(IDS_YNBADCTRLDIGIT,MB_YESNO,0) == IDNO)
		{
			m_cowno.SetFocus();
			return false;
		}
	}

	
	NullDecimal weight = m_weight.GetDecimal();

	if(weight.IsNull())
	{
		AfxMessageBox(IDS_ERRREQWEIGHT,MB_OK | MB_ICONERROR,0);
		m_weight.SetFocus();
		return false;
	}

	if(weight <= NullDecimal(0L))
	{
		AfxMessageBox(IDS_ERRNEEDPOSITIVEWEIGHT,MB_OK | MB_ICONERROR,0);
		m_weight.SetFocus();
		return false;
	}
	
	if(m_class.GetSelectedItemId() == NULL_ID)
	{		
		AfxMessageBox(IDS_ERRSELCLASS,MB_OK | MB_ICONERROR,0);
		m_class.SetFocus();
		return false;
	}
	
	/*if(!m_passno.IsFullPrompt()) 
	{		
		if(AfxMessageBox(IDS_ASKEMPTYPASSNO,MB_YESNO,0) != IDYES)
		{	
			m_passno.SetFocus();
			return false;
		}
	}*/
	
	CowNo motherNo = m_motherean.GetCowNo();
	if(!motherNo.IsNull()) 
	{
		if(!CowNoValidator::IsValid(motherNo))
		{
			if(AfxMessageBox(IDS_YNBADCTRLDIGIT,MB_YESNO,0) == IDNO)
			{
				m_motherean.SetFocus();
				return false;
			}
		}
	}
	
	if(!m_birthdate.IsInputEmpty() && !m_birthdate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADDATE,MB_OK | MB_ICONERROR,0);
		return false;
	}

	if(!m_passdate.IsInputEmpty() && !m_passdate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADDATE,MB_OK | MB_ICONERROR,0);
		return false;
	}	
	
	return true;
}

void CowDlg::SaveCow()
{	
	m_pEditor->SetCowNo(m_cowno.GetCowNo());	
	m_pEditor->SetWeight(m_weight.GetDecimal().ToDecimal(Decimal(0L)));	

	std::wstring sPassNo;
	if(m_passno.IsFullPrompt())
		sPassNo = m_passno.GetText();

	m_pEditor->SetPassNo(sPassNo);

	m_pEditor->SetStockId(m_stock.GetSelectedItemId());
	m_pEditor->SetClassId(m_class.GetSelectedItemId());
	m_pEditor->SetBirthDate(m_birthdate.GetDate());
	m_pEditor->SetBirthPlace(m_birthplace);	
	m_pEditor->SetExtras(m_extras.GetText());
	m_pEditor->SetPassDate(m_passdate.GetDate());
	m_pEditor->SetFirstOwnerId(m_hentbtn.GetHent());	
	m_pEditor->SetMotherNo(m_motherean.GetCowNo());
	m_pEditor->SetSex(m_sexbtns.GetSex());	
	m_pEditor->SetHealthCertNo(m_HealthCertNo);		
}

void CowDlg::OnOK() 
{
	if(!Validate()) return;
	
	SaveCow();

	try
	{
		m_pEditor->Save();
		EndDialog(IDOK);
	}
	catch(wexception& e)
	{
		AfxMessageBox(e.wwhat(),MB_OK | MB_ICONERROR);
	}	
}

void CowDlg::OnCowClassChange()
{
	/*Stock *pStock = m_stock.GetSelItemData();
	if(pStock && !pStock->GetPredefSex().IsUNK())
	{
		m_combosex.SelSex(pStock->GetPredefSex().getCOWSEX());

		CWnd* sexCombo = GetDlgItem(IDC_COMBOSEX);
		CRect rect;
		sexCombo->GetClientRect(&rect);
		sexCombo->ClientToScreen(&rect);

		m_UsedPreDefinedSexTip.Show(CPoint(rect.left,rect.bottom),
									CTempStringLoader(IDS_USEDPREDEFSEX),
									PREDEFSEX_SETTING_NOTIFY_DELAY);

	}*/
}

void CowDlg::OnClassComboLostFocus()
{
	//m_UsedPreDefinedSexTip.Hide();
}

void CowDlg::OnFirstOwner()
{
	HentSearchDlg hentSearchDlg(this);
	uint32_t hentId = hentSearchDlg.FindHent();
	if (hentId != NULL_ID)
	{
		m_hentbtn.SetHent(hentId);
	}
}

void CowDlg::OnScanCowNo()
{
	CowNoBarcodeInputDialog cowNoInputDlg(this);
	CowNo cowNo = cowNoInputDlg.InputCowNo();
	if (!cowNo.IsNull())
	{
		m_cowno.SetCowNo(cowNo);
	}

}
