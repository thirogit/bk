#include "stdafx.h"
#include "InvoiceHentUpdateDlg.h"
#include <data\datalimits.h>

BEGIN_MESSAGE_MAP(CInvoiceHentUpdateDlg,CDialog)
	
	ON_BN_CLICKED(IDC_UPDATE_NAME,OnUpdateName)
    ON_BN_CLICKED(IDC_UPDATE_STREETPOBOX,OnUpdateStreetPOBox)
    ON_BN_CLICKED(IDC_UPDATE_CITYZIP,OnUpdateCityZip)
    ON_BN_CLICKED(IDC_UPDATE_HENTNO,OnUpdateHentNo)
    ON_BN_CLICKED(IDC_UPDATE_FISCALNO,OnUpdateFiscalNo)
    ON_BN_CLICKED(IDC_UPDATE_ACCOUNTNO,OnUpdateAccountNo)
    ON_BN_CLICKED(IDC_UPDATE_BANK,OnUpdateBank)
    ON_BN_CLICKED(IDC_UPDATE_PESEL,OnUpdatePersonalNo)
    ON_BN_CLICKED(IDC_UPDATE_PERSONALID_NO,OnUpdatePersonalIdNo)
    ON_BN_CLICKED(IDC_UPDATE_PERSONALID_ISSUEDATE,OnUpdatePersonalIdIssueDt)
    ON_BN_CLICKED(IDC_UPDATE_PERSONALID_ISSUEPOST,OnUpdatePersonalIdIssuePost)
	ON_EN_CHANGE(IDC_INVSIDE_NAME,OnNameChange)
	
	ON_EN_CHANGE(IDC_INVSIDE_STREET,OnStreetChange)
	ON_EN_CHANGE(IDC_INVSIDE_POBOX,OnPOBoxChange)
	ON_EN_CHANGE(IDC_INVSIDE_CITY,OnCityChange)
	ON_EN_CHANGE(IDC_INVSIDE_ZIP,OnZipChange)
	ON_EN_CHANGE(IDC_INVSIDE_HENTNO,OnHentNoChange)
	ON_EN_CHANGE(IDC_INVSIDE_FISCALNO,OnFiscalNoChange)
	ON_EN_CHANGE(IDC_INVSIDE_ACCOUNTNO,OnAccountNoChange)
	ON_EN_CHANGE(IDC_INVSIDE_BANK,OnBankChange)
	ON_EN_CHANGE(IDC_INVSIDE_PERSONALNO,OnPersonalNoChange)
	ON_EN_CHANGE(IDC_INVSIDE_PERSONALID_NO,OnPersonalIdNoChange)
	ON_EN_CHANGE(IDC_INVSIDE_PERSONALID_ISSUEDATE,OnPersonalIdIssueDtChange)
	ON_EN_CHANGE(IDC_INVSIDE_PERSONALID_ISSUEPOST,OnPersonalIdIssuePostChange)


END_MESSAGE_MAP()

CInvoiceHentUpdateDlg::CInvoiceHentUpdateDlg(const InvoiceHent& toUpdate,const InvoiceHent& reference) : CDialog(CInvoiceHentUpdateDlg::IDD)
{
	m_hentToUpdate.CopyFrom(toUpdate);
	m_reference.CopyFrom(reference);
}

CInvoiceHentUpdateDlg::~CInvoiceHentUpdateDlg()
{

}

void CInvoiceHentUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX,IDC_INVSIDE_NAME,m_invoiceSide.name);
	DDX_Control(pDX,IDC_INVSIDE_STREET,m_invoiceSide.street);
	DDX_Control(pDX,IDC_INVSIDE_POBOX,m_invoiceSide.POBox);
	DDX_Control(pDX,IDC_INVSIDE_CITY,m_invoiceSide.city);
	DDX_Control(pDX,IDC_INVSIDE_ZIP,m_invoiceSide.zip);
	DDX_Control(pDX,IDC_INVSIDE_HENTNO,m_invoiceSide.hentNo);
	DDX_Control(pDX,IDC_INVSIDE_FISCALNO,m_invoiceSide.fiscalNo);
	DDX_Control(pDX,IDC_INVSIDE_ACCOUNTNO,m_invoiceSide.accountNo);
	DDX_Control(pDX,IDC_INVSIDE_BANK,m_invoiceSide.bank);
	DDX_Control(pDX,IDC_INVSIDE_PERSONALNO,m_invoiceSide.personalNo);
	DDX_Control(pDX,IDC_INVSIDE_PERSONALID_NO,m_invoiceSide.personalIdNo);
	DDX_Control(pDX,IDC_INVSIDE_PERSONALID_ISSUEDATE,m_invoiceSide.personalIdIssueDt);
	DDX_Control(pDX,IDC_INVSIDE_PERSONALID_ISSUEPOST,m_invoiceSide.personalIdIssuePost);
	DDX_Control(pDX,IDC_REFSIDE_NAME,m_referenceSide.name);
	DDX_Control(pDX,IDC_REFSIDE_STREET,m_referenceSide.street);
	DDX_Control(pDX,IDC_REFSIDE_POBOX,m_referenceSide.POBox);
	DDX_Control(pDX,IDC_REFSIDE_CITY,m_referenceSide.city);
	DDX_Control(pDX,IDC_REFSIDE_ZIP,m_referenceSide.zip);
	DDX_Control(pDX,IDC_REFSIDE_HENTNO,m_referenceSide.hentNo);
	DDX_Control(pDX,IDC_REFSIDE_FISCALNO,m_referenceSide.fiscalNo);
	DDX_Control(pDX,IDC_REFSIDE_ACCOUNTNO,m_referenceSide.accountNo);
	DDX_Control(pDX,IDC_REFSIDE_BANK,m_referenceSide.bank);
	DDX_Control(pDX,IDC_REFSIDE_PERSONALNO,m_referenceSide.personalNo);
	DDX_Control(pDX,IDC_REFSIDE_PERSONALID_NO,m_referenceSide.personalIdNo);
	DDX_Control(pDX,IDC_REFSIDE_PERSONALID_ISSUEDATE,m_referenceSide.personalIdIssueDt);
	DDX_Control(pDX,IDC_REFSIDE_PERSONALID_ISSUEPOST,m_referenceSide.personalIdIssuePost);

}

 BOOL CInvoiceHentUpdateDlg::OnInitDialog()
 {
	CDialog::OnInitDialog();
	SetSide(m_referenceSide,m_reference);
	m_referenceSide.hentNo.EnableButton();

	SetSide(m_invoiceSide,m_hentToUpdate);
	m_invoiceSide.hentNo.EnableButton();

	SetReadOnly(m_referenceSide,TRUE);

	m_invoiceSide.name.SetLimitText(MAXNAME);	
	m_invoiceSide.street.SetLimitText(MAXSTREET);	
	m_invoiceSide.POBox.SetLimitText(MAXPOBOX);	
	m_invoiceSide.city.SetLimitText(MAXCITY);	
	m_invoiceSide.bank.SetLimitText(MAXBANKNAME);	
	m_invoiceSide.personalIdIssuePost.SetLimitText(MAXISSUEPOST);

	return TRUE;
 }

 void CInvoiceHentUpdateDlg::SetReadOnly(InvoiceHentSide& side,BOOL bReadOnly)
 {
	 side.name.SetReadOnly(bReadOnly);
	 side.street.SetReadOnly(bReadOnly);
	 side.POBox.SetReadOnly(bReadOnly);
	 side.city.SetReadOnly(bReadOnly);
	 side.zip.SetReadOnly(bReadOnly);
	 side.hentNo.SetReadOnly(bReadOnly);
	 side.fiscalNo.SetReadOnly(bReadOnly);
	 side.accountNo.SetReadOnly(bReadOnly);
	 side.bank.SetReadOnly(bReadOnly);
	 side.personalNo.SetReadOnly(bReadOnly);
	 side.personalIdNo.SetReadOnly(bReadOnly);
	 side.personalIdIssueDt.SetReadOnly(bReadOnly);
	 side.personalIdIssuePost.SetReadOnly(bReadOnly);
 }

 void CInvoiceHentUpdateDlg::SetSide(InvoiceHentSide& side,const InvoiceHent& hent)
 {
	 side.name.SetText(hent.GetName());
	 side.street.SetText(hent.GetStreet());
	 side.POBox.SetText(hent.GetPOBox());
	 side.city.SetText(hent.GetCity());
	 side.zip.SetText(hent.GetZip());
	 side.hentNo.SetHentNo(hent.GetHentNo());
	 side.fiscalNo.SetText(hent.GetFiscalNo());
	 side.accountNo.SetAccountNo(hent.GetAccountNo());
	 side.bank.SetText(hent.GetBankName());
	 side.personalNo.SetText(hent.GetPESEL());
	 side.personalIdNo.SetText(hent.GetIdNo());
	 side.personalIdIssueDt.SetDate(hent.GetIssueDate());
	 side.personalIdIssuePost.SetText(hent.GetIssuePost());
 }


void CInvoiceHentUpdateDlg::OnUpdateName()
{
	m_invoiceSide.name.SetText(m_reference.GetName());
}

void CInvoiceHentUpdateDlg::OnNameChange()
{
	if (m_referenceSide.name.GetText() != m_invoiceSide.name.GetText())
	{
		m_referenceSide.name.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.name.SetBkgColor(Color());
	}
	
}

void CInvoiceHentUpdateDlg::OnUpdateStreetPOBox()
{
	m_invoiceSide.street.SetText(m_reference.GetStreet());
	m_invoiceSide.POBox.SetText(m_reference.GetPOBox());	
}

void CInvoiceHentUpdateDlg::OnStreetChange()
{
	if (m_referenceSide.street.GetText() != m_invoiceSide.street.GetText())
	{
		m_referenceSide.street.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.street.SetBkgColor(Color());
	}	
}

void CInvoiceHentUpdateDlg::OnPOBoxChange()
{
	if (m_referenceSide.POBox.GetText() != m_invoiceSide.POBox.GetText())
	{
		m_referenceSide.POBox.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.POBox.SetBkgColor(Color());
	}
	
}
	
void CInvoiceHentUpdateDlg::OnUpdateCityZip()
{
	m_invoiceSide.city.SetText(m_reference.GetCity());
	m_invoiceSide.zip.SetText(m_reference.GetZip());	
}

void CInvoiceHentUpdateDlg::OnCityChange()
{
	if (m_referenceSide.city.GetText() != m_invoiceSide.city.GetText())
	{
		m_referenceSide.city.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.city.SetBkgColor(Color());
	}
	
}

void CInvoiceHentUpdateDlg::OnZipChange()
{
	if (m_referenceSide.zip.GetText() != m_invoiceSide.zip.GetText())
	{
		m_referenceSide.zip.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.zip.SetBkgColor(Color());
	}
	
}

void CInvoiceHentUpdateDlg::OnUpdateHentNo()
{
	m_invoiceSide.hentNo.SetHentNo(m_reference.GetHentNo());	
}
	
void CInvoiceHentUpdateDlg::OnHentNoChange()
{

	if (!m_invoiceSide.hentNo.IsFullPrompt() || m_referenceSide.hentNo.GetHentNo() != m_invoiceSide.hentNo.GetHentNo())
	{
		m_referenceSide.hentNo.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.hentNo.SetBkgColor(Color());
	}
	
}

void CInvoiceHentUpdateDlg::OnUpdateFiscalNo()
{
	m_invoiceSide.fiscalNo.SetText(m_reference.GetFiscalNo());
}

void CInvoiceHentUpdateDlg::OnFiscalNoChange()
{
	if (m_referenceSide.fiscalNo.GetText() != m_invoiceSide.fiscalNo.GetText())
	{
		m_referenceSide.fiscalNo.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.fiscalNo.SetBkgColor(Color());
	}	
}

void CInvoiceHentUpdateDlg::OnUpdateAccountNo()
{
	m_invoiceSide.accountNo.SetAccountNo(m_reference.GetAccountNo());
}

void CInvoiceHentUpdateDlg::OnAccountNoChange()
{	
	if (m_invoiceSide.accountNo.IsInputEmpty() && m_referenceSide.accountNo.GetAccountNo().IsNull())
	{
		m_referenceSide.accountNo.SetBkgColor(Color());
	} 
	else if (!m_invoiceSide.accountNo.IsFullPrompt() || m_referenceSide.accountNo.GetAccountNo() != m_invoiceSide.accountNo.GetAccountNo())
	{
		m_referenceSide.accountNo.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.accountNo.SetBkgColor(Color());
	}
	
}

void CInvoiceHentUpdateDlg::OnUpdateBank()
{
	m_invoiceSide.bank.SetText(m_reference.GetBankName());	
}

void CInvoiceHentUpdateDlg::OnBankChange()
{
	if (m_referenceSide.bank.GetText() != m_invoiceSide.bank.GetText())
	{
		m_referenceSide.bank.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.bank.SetBkgColor(Color());
	}	
}

void CInvoiceHentUpdateDlg::OnUpdatePersonalNo()
{
	m_invoiceSide.personalNo.SetText(m_reference.GetPESEL());	
}

void CInvoiceHentUpdateDlg::OnPersonalNoChange()
{
	if (m_referenceSide.personalNo.GetText() != m_invoiceSide.personalNo.GetText())
	{
		m_referenceSide.personalNo.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.personalNo.SetBkgColor(Color());
	}	
}

void CInvoiceHentUpdateDlg::OnUpdatePersonalIdNo()
{
	m_invoiceSide.personalIdNo.SetText(m_reference.GetIdNo());	
}

void CInvoiceHentUpdateDlg::OnPersonalIdNoChange()
{
	if (m_referenceSide.personalIdNo.GetText() != m_invoiceSide.personalIdNo.GetText())
	{
		m_referenceSide.personalIdNo.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.personalIdNo.SetBkgColor(Color());
	}
	
}

void CInvoiceHentUpdateDlg::OnUpdatePersonalIdIssueDt()
{
	m_invoiceSide.personalIdIssueDt.SetDate(m_reference.GetIssueDate());	
}

void CInvoiceHentUpdateDlg::OnPersonalIdIssueDtChange()
{
	if (m_invoiceSide.personalIdIssueDt.IsInputEmpty() && m_referenceSide.personalIdIssueDt.GetDate().IsNull())
	{
		m_referenceSide.personalIdIssueDt.SetBkgColor(Color());
		return;
	}

	if (!m_invoiceSide.personalIdIssueDt.IsFullPrompt())
	{
		m_referenceSide.personalIdIssueDt.SetBkgColor(Color(DIFFERENCE_COLOR));
		return;
	}

	if (!m_invoiceSide.personalIdIssueDt.IsDateValid() || (m_referenceSide.personalIdIssueDt.GetDate() != m_invoiceSide.personalIdIssueDt.GetDate()))
	{
		m_referenceSide.personalIdIssueDt.SetBkgColor(Color(DIFFERENCE_COLOR));
		return;
	}
	
	
	m_referenceSide.personalIdIssueDt.SetBkgColor(Color());
	
}

void CInvoiceHentUpdateDlg::OnUpdatePersonalIdIssuePost()
{
	m_invoiceSide.personalIdIssuePost.SetText(m_reference.GetIssuePost());	
}

void CInvoiceHentUpdateDlg::OnPersonalIdIssuePostChange()
{
	if (m_referenceSide.personalIdIssuePost.GetText() != m_invoiceSide.personalIdIssuePost.GetText())
	{
		m_referenceSide.personalIdIssuePost.SetBkgColor(Color(DIFFERENCE_COLOR));
	}
	else
	{
		m_referenceSide.personalIdIssuePost.SetBkgColor(Color());
	}
	
}

const InvoiceHent& CInvoiceHentUpdateDlg::GetUpdatedHent() const
{
	return m_hentToUpdate;
}

void CInvoiceHentUpdateDlg::OnOK()
{
	if(m_hentToUpdate.GetHentNo().IsNull()) 
	{
		AfxMessageBox(IDS_ERREMPTYHENTNO);
		m_invoiceSide.hentNo.SetFocus();
		return;
	}
	
	if(m_hentToUpdate.GetName().empty())
    {
		AfxMessageBox(IDS_ERREMPTYNAME);
		m_invoiceSide.name.SetFocus();
		return;
	}
		
	if(m_hentToUpdate.GetStreet().empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYSTREET);
		m_invoiceSide.street.SetFocus();
		return;
	}

	if(m_hentToUpdate.GetPOBox().empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYPOBOX);
		m_invoiceSide.POBox.SetFocus();
		return;
	}

	if(m_hentToUpdate.GetCity().empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYCITY);
		m_invoiceSide.city.SetFocus();
		return;
	}

	if(m_hentToUpdate.GetZip().empty()) 
	{
		AfxMessageBox(IDS_ERREMPTYZIP);
		m_invoiceSide.zip.SetFocus();
		return;
	}		
	
	/*if(!m_idno.IsInputEmpty() && !m_idno.IsInputValid())
	{
		AfxMessageBox(IDS_ERRBADIDNO);
		m_idno.SetFocus();
		return false;
	}*/

	if(!m_invoiceSide.personalIdIssueDt.IsInputEmpty() && !m_invoiceSide.personalIdIssueDt.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADISSUEDATE);
		m_invoiceSide.personalIdIssueDt.SetFocus();
		return;
	}

	
	m_hentToUpdate.SetName(m_invoiceSide.name.GetText());
	m_hentToUpdate.SetStreet(m_invoiceSide.street.GetText());
	m_hentToUpdate.SetPOBox(m_invoiceSide.POBox.GetText());
	m_hentToUpdate.SetCity(m_invoiceSide.city.GetText());
	m_hentToUpdate.SetZip(m_invoiceSide.zip.GetText());
	m_hentToUpdate.SetHentNo(m_invoiceSide.hentNo.GetHentNo());
	m_hentToUpdate.SetFiscalNo(m_invoiceSide.fiscalNo.GetText());
	m_hentToUpdate.SetAccountNo(m_invoiceSide.accountNo.GetAccountNo());
	m_hentToUpdate.SetBankName(m_invoiceSide.bank.GetText());
	m_hentToUpdate.SetPESEL(m_invoiceSide.personalNo.GetText());
	m_hentToUpdate.SetIdNo(m_invoiceSide.personalIdNo.GetText());
	m_hentToUpdate.SetIssueDate(m_invoiceSide.personalIdIssueDt.GetDate());
	m_hentToUpdate.SetIssuePost(m_invoiceSide.personalIdIssuePost.GetText());

	EndDialog(IDOK);
}