#include "stdafx.h"
#include "ListBoxSelectDlg.h"
#include <data\IIdable.h>

BEGIN_MESSAGE_MAP(CListBoxSelectDlg, CDialog)
END_MESSAGE_MAP()

CListBoxSelectDlg::CListBoxSelectDlg(CWnd* pParent /*=NULL*/): CDialog(CListBoxSelectDlg::IDD, pParent)
{
	m_selectedItemId = NULL;
}

CListBoxSelectDlg::~CListBoxSelectDlg()
{
}

void CListBoxSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BOOL CListBoxSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(!m_sCaption.empty())
		SetWindowText(m_sCaption.c_str());

	int defaultItemIndex = -1;
	PtrEnumerator<ComboItem> enumItems = m_content.EnumItems();
	while (enumItems.hasNext())
	{
		ComboItem* item = *enumItems;
		int index = m_list.AddString(item->GetItemText().c_str());
		m_list.SetItemData(index, item->GetItemId());

		if (m_defaultItem == item->GetItemId())
		{
			defaultItemIndex = index;
		}
		enumItems.advance();
	}

	m_list.SetCurSel(defaultItemIndex);	
	
	m_selectedItemId = NULL_ID;
		
	return TRUE;
}

void CListBoxSelectDlg::SetCaption(const std::wstring& sCaption)
{
	m_sCaption = sCaption;
}

void CListBoxSelectDlg::OnOK()
{	
	m_selectedItemId = GetSelectedItemId();
	if (m_selectedItemId != NULL_ID)
	{
		EndDialog(IDOK);
	}
}

uint32_t CListBoxSelectDlg::GetSelectedItemId()
{
	int nSelectedItem = m_list.GetCurSel();
	if (nSelectedItem < 0) return NULL_ID;
	return (uint32_t)m_list.GetItemData(nSelectedItem);
}

uint32_t CListBoxSelectDlg::Select(ComboContent& content, uint32_t itemId)
{
	m_content = content;
	m_defaultItem = itemId;
	
	if (DoModal() == IDOK)
	{
		return m_selectedItemId;
	}
	return NULL_ID;
}


