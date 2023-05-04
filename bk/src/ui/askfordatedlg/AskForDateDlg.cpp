#include "stdafx.h"
#include "AskForDateDlg.h"
#include "../ExceptionMsgBox.h"

IMPLEMENT_DYNAMIC(CAskForDateDlg, CDialog)

BEGIN_MESSAGE_MAP(CAskForDateDlg, CDialog)
END_MESSAGE_MAP()

CAskForDateDlg::CAskForDateDlg(CWnd* pParent /*=NULL*/): CDialog(CAskForDateDlg::IDD, pParent)
{
	m_dt.MakeNull();
}

CAskForDateDlg::~CAskForDateDlg()
{
}

void CAskForDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_INPUTEDIT,m_dateEdit);
}


BOOL CAskForDateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(!m_sCaption.empty())
		SetWindowText(m_sCaption.c_str());

	if(!m_sLabel.empty())
		SetDlgItemText(IDC_INPUTLABEL,m_sLabel.c_str());

	if (!m_dt.IsNull())
	{
		m_dateEdit.SetDate(m_dt);
		m_dateEdit.SetSel(0, -1);
	}
	else
	{
		m_dateEdit.SetSel(0, 0);
	}
	
	return TRUE;
}

void CAskForDateDlg::SetLabel(const std::wstring& sLabel)
{
	m_sLabel = sLabel;
}

void CAskForDateDlg::SetWindowCaption(const std::wstring& sCaption)
{
	m_sCaption = sCaption;
}

void CAskForDateDlg::SetDate(const DateTime& dt)
{
	m_dt = dt;
}


void CAskForDateDlg::OnOK()
{
	DateTime dt = m_dateEdit.GetDate();
	
	if(!dt.IsNull())
	{
		
		if(m_validator) 
		{
			ExceptionMsgBox msgBox;
			msgBox.TryCatchAny(std::bind(m_validator,dt));				
		}
			
		m_dt = dt;
		EndDialog(IDOK);
		return;		
		
	}
	
}

DateTime CAskForDateDlg::AskForDate()
{
	if(DoModal() == IDOK)
		return m_dt;

	return DateTime();

}



