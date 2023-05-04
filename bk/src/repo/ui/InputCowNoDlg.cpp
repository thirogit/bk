#include "stdafx.h"
#include "InputCowNoDlg.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"
#include <validation\CowNoValidator.h>

BEGIN_MESSAGE_MAP(CInputCowNoDlg, CDialog)
END_MESSAGE_MAP()

CInputCowNoDlg::CInputCowNoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputCowNoDlg::IDD, pParent)
{
	
}

CInputCowNoDlg::~CInputCowNoDlg()
{
}

void CInputCowNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDITBOX,m_CowNoEdit);
}

BOOL CInputCowNoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::wstring sCaption = StringLoader(IDS_ASKFORCOWNOCAPTION);
	std::wstring sLabel = StringLoader(IDS_ASKFORCOWNOLABEL);

	SetWindowText(sCaption.c_str());
	SetDlgItemText(IDC_EDITBOXDLGLBL,sLabel.c_str());
	m_CowNoEdit.SetCowNo(m_CowNo);	

	return TRUE;
}


void CInputCowNoDlg::OnOK()
{
	CowNo cowNo = m_CowNoEdit.GetCowNo();
	
	if (!cowNo.IsNull())
	{
		if (!CowNoValidator::IsValid(cowNo))
		{
			if (AfxMessageBox(IDS_YNBADCTRLDIGIT, MB_YESNO, 0) == IDNO)
			{
				m_CowNoEdit.SetFocus();
				return;
			}
		}

		m_CowNo = cowNo;
		EndDialog(IDOK);
		return;
	}
}


CowNo CInputCowNoDlg::ask()
{
	if(DoModal() == IDOK)
		return m_CowNo;

	return CowNo();

}





