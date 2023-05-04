#include "stdafx.h"
#include "DeductionDetailsDlg.h"
#include <data\datalimits.h>
#include <string\TextUtils.h>

BEGIN_MESSAGE_MAP(CDeductionDetailsDlg,CDialog)

END_MESSAGE_MAP()

CDeductionDetailsDlg::CDeductionDetailsDlg(CWnd* pParent/* = NULL*/) : CDialog(CDeductionDetailsDlg::IDD,pParent), m_fraction(3)
{
}

CDeductionDetailsDlg::CDeductionDetailsDlg(const DeductionDetails& details, CWnd* pParent/* = NULL*/) : CDeductionDetailsDlg(pParent)
{
	m_details.CopyFrom(details);
}

CDeductionDetailsDlg::~CDeductionDetailsDlg()
{
}

void CDeductionDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_FRACTION, m_fraction);
	DDX_Control(pDX, IDC_DEDUCTIONCODE, m_code);
	DDX_Control(pDX, IDC_REASON, m_reason);
}

 BOOL CDeductionDetailsDlg::OnInitDialog()
 {
	CDialog::OnInitDialog();

	m_fraction.SetDecimal(m_details.GetFraction()*Decimal(100L));
	m_code.SetText(m_details.GetCode());
	m_code.SetLimitText(MAXDEDUCTIONCODE);
	m_reason.SetText(m_details.GetReason());
	m_reason.SetLimitText(MAXDEDUCTIONREASON);	

	SetWindowText(GetCaption().c_str()); 
	return TRUE;
 }

 const DeductionDetails& CDeductionDetailsDlg::GetDeducationDetails() const
{
	 return m_details;
}

void CDeductionDetailsDlg::OnOK()
{
	NullDecimal fraction = m_fraction.GetDecimal();

	if (fraction.IsNull())
	{
		AfxMessageBox(IDS_ERRBADDEDUCTIONFRACTION);
		m_fraction.SetFocus();
		return;
	}

	std::wstring code = m_code.GetText();

	if (code.empty() || TextUtils::IsAllWhitespace(code))
	{
		AfxMessageBox(IDS_EMPTYDEDUCTIONCODE);
		m_code.SetFocus();
		return;
	}

	std::wstring reason = m_reason.GetText();

	if (reason.empty() || TextUtils::IsAllWhitespace(reason))
	{
		AfxMessageBox(IDS_ERRBADDEDUCTIONREASON);
		m_reason.SetFocus();
		return;
	}

	m_details.SetFraction(fraction.ToDecimal(Decimal(0L))/Decimal(100L));
	m_details.SetCode(code);
	m_details.SetReason(reason);
	
	EndDialog(IDOK);
}