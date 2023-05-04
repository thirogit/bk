#include "stdafx.h"
#include "InputWeightDlg.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"

BEGIN_MESSAGE_MAP(CInputWeightDlg, CDialog)
END_MESSAGE_MAP()

CInputWeightDlg::CInputWeightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputWeightDlg::IDD, pParent)
{
	
}

CInputWeightDlg::~CInputWeightDlg()
{
}

void CInputWeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDITBOX,m_WeightEdit);

}

BOOL CInputWeightDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::wstring sCaption = StringLoader(IDS_ASKFORWEIGHTCAPTION);
	std::wstring sLabel = StringLoader(IDS_ASKFORWEIGHTLABEL);

	SetWindowText(sCaption.c_str());
	SetDlgItemText(IDC_EDITBOXDLGLBL,sLabel.c_str());
	m_WeightEdit.SetDecimal(m_Weight);	
	m_validationMsg.Create(this);

	return TRUE;
}


void CInputWeightDlg::OnOK()
{
	NullDecimal weight;
	weight = m_WeightEdit.GetDecimal();
		
	if (m_validator)
	{
		try
		{
			m_validator(weight);
		}
		catch (WeightValidationException& e)
		{
			CRect rect;
			GetDlgItem(IDC_EDITBOX)->GetWindowRect(&rect);
			m_validationMsg.SetPosition(CPoint(rect.left, rect.bottom));
			m_validationMsg.SetText(e.wwhat());
			m_validationMsg.Show(3000);
		}		
		return;
	}
	else
	{
		m_Weight = weight;
		EndDialog(IDOK);
		return;
	}	
}


NullDecimal CInputWeightDlg::ask()
{
	if(DoModal() == IDOK)
		return m_Weight;

	return NullDecimal();

}

CInputWeightDlg& CInputWeightDlg::weight(const NullDecimal& weight)
{
	m_Weight = weight;
	return *this;
}

CInputWeightDlg& CInputWeightDlg::validator(WeightValidator& validator)
{
	m_validator = validator;
	return *this;
}



