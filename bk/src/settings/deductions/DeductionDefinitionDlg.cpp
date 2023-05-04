#include "stdafx.h"
#include "DeductionDefinitionDlg.h"
#include <data\datalimits.h>
#include <string\TextUtils.h>
#include "../../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CDeductionDefinitionDlg,CDialog)

END_MESSAGE_MAP()

CDeductionDefinitionDlg::CDeductionDefinitionDlg(CRUDDeductionDefinition* definition, CWnd* pParent/* = NULL*/) :
CDialog(CDeductionDefinitionDlg::IDD, pParent), m_fraction(3), m_definition(definition)
{
}

CDeductionDefinitionDlg::~CDeductionDefinitionDlg()
{
}

void CDeductionDefinitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_FRACTION, m_fraction);
	DDX_Control(pDX, IDC_DEDUCTIONCODE, m_code);
	DDX_Control(pDX, IDC_REASON, m_reason);
	DDX_Check(pDX, IDC_ALWAYSENABLED, m_alwaysEnabled);
}

 BOOL CDeductionDefinitionDlg::OnInitDialog()
 {
	CDialog::OnInitDialog();

	m_fraction.SetDecimal(m_definition->GetFraction());
	m_code.SetText(m_definition->GetCode());
	m_code.SetLimitText(MAXDEDUCTIONCODE);
	m_reason.SetText(m_definition->GetReason());
	m_reason.SetLimitText(MAXDEDUCTIONREASON);	
	m_alwaysEnabled = m_definition->GetIsEnabled() ? TRUE : FALSE;

	SetWindowText(StringLoader(m_definition->GetFlag() != CRUDFlag_New ? IDS_EDITDEDUCTIONDEFINITION : IDS_NEWDEDUCTIONDEFINITION).c_str());

	UpdateData(FALSE);
	
	return TRUE;
 }

void CDeductionDefinitionDlg::OnOK()
{
	UpdateData();
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

	m_definition->SetFraction(fraction.ToDecimal(Decimal(0L)));
	m_definition->SetCode(code);
	m_definition->SetReason(reason);
	m_definition->SetIsEnabled(m_alwaysEnabled == TRUE);

	EndDialog(IDOK);
}