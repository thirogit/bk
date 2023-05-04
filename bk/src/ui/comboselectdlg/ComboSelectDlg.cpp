#include "stdafx.h"
#include "ComboSelectDlg.h"
#include <data\IIdable.h>

BEGIN_MESSAGE_MAP(CComboSelectDlg, CDialog)
END_MESSAGE_MAP()

CComboSelectDlg::CComboSelectDlg(CWnd* pParent /*=NULL*/): CDialog(CComboSelectDlg::IDD, pParent)
{
	m_selectedItemId = NULL;
}

CComboSelectDlg::~CComboSelectDlg()
{
}

void CComboSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_combo);
}

BOOL CComboSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(!m_sCaption.empty())
		SetWindowText(m_sCaption.c_str());

	m_combo.SetContent(m_content);
	m_combo.Select(m_defaultItem);
	m_selectedItemId = NULL_ID;
		
	return TRUE;
}

void CComboSelectDlg::SetCaption(const std::wstring& sCaption)
{
	m_sCaption = sCaption;
}

void CComboSelectDlg::OnOK()
{	
	m_selectedItemId = m_combo.GetSelectedItemId();
	if (m_selectedItemId != NULL_ID)
	{
		EndDialog(IDOK);
	}
}

uint32_t CComboSelectDlg::Select(ComboContent& content, uint32_t itemId)
{
	m_content = content;
	m_defaultItem = itemId;
	
	if (DoModal() == IDOK)
	{
		return m_selectedItemId;
	}
	return NULL_ID;
}


