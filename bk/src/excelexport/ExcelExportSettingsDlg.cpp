#include "stdafx.h"
#include "ExcelExportSettingsDlg.h"
#include "../header/CowPresenceListHeader.h"

ExcelExportSettingsDlg::ExcelExportSettingsDlg(ExcelExportSettings* settings,const ListHeaderDescriptor* headerDescriptor,CWnd* pParent /*=NULL*/)
	: CDialog(ExcelExportSettingsDlg::IDD, pParent),m_Settings(settings),m_HeaderDescriptor(headerDescriptor)
{
}

ExcelExportSettingsDlg::~ExcelExportSettingsDlg()
{
	
}

void ExcelExportSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LEFTLIST, m_LeftList);
	DDX_Control(pDX,IDC_RIGHTLIST, m_RightList);
}


BEGIN_MESSAGE_MAP(ExcelExportSettingsDlg, CDialog)
	
	ON_BN_CLICKED(IDC_MOVEUP,OnMoveUp)
	ON_BN_CLICKED(IDC_MOVEDOWN,OnMoveDown)
	ON_BN_CLICKED(IDC_MOVETOLEFT,OnMoveToLeft)
	ON_BN_CLICKED(IDC_MOVETORIGHT,OnMoveToRight)
END_MESSAGE_MAP()


BOOL ExcelExportSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	PtrEnumerator<HeaderColumn> columnEnum = m_HeaderDescriptor->EnumColumns();
	
	int iItem;
	while (columnEnum.hasNext())
	{
		if (!m_Settings->HasField(columnEnum->GetColId()))
		{
			iItem = m_LeftList.InsertString(-1, columnEnum->GetColText().c_str());
			m_LeftList.SetItemData(iItem, (DWORD_PTR)columnEnum->GetColId());
		}		
		columnEnum.advance();
	}
	
	for (int i = 0, count = m_Settings->GetFieldCount(); i < count; i++)
	{
		uint32_t colId = m_Settings->GetField(i);
		const HeaderColumn* column = m_HeaderDescriptor->GetColumnWithId(colId);
		iItem = m_RightList.InsertString(-1, column->GetColText().c_str());
		m_RightList.SetItemData(iItem, (DWORD_PTR)column->GetColId());
	}


	return TRUE;
}

bool ExcelExportSettingsDlg::Configure()
{
	return (DoModal() == IDOK);
}

void ExcelExportSettingsDlg::OnOK()
{
	if (m_RightList.GetCount() > 0)
	{
		UpdateData();
		m_Settings->RemoveAllFields();

		for (int i = 0, count = m_RightList.GetCount(); i < count; i++)
		{
			uint32_t fieldId = m_RightList.GetItemData(i);
			m_Settings->AddField(fieldId);
		}
		EndDialog(IDOK);
	}
	else
	{
		AfxMessageBox(IDS_ERRNOFIELDSELECTED, MB_OK | MB_ICONERROR, 0);
	}

}


void ExcelExportSettingsDlg::OnMoveUp()
{
	MoveItemUp(m_RightList.GetCurSel());
}

void ExcelExportSettingsDlg::OnMoveDown()
{
	MoveItemDown(m_RightList.GetCurSel());
}

void ExcelExportSettingsDlg::OnMoveToLeft()
{
	int selected = m_RightList.GetCurSel();

	if (selected >= 0)
	{
		std::wstring itemText = GetItemText(m_RightList, selected);		
		DWORD itemData = m_RightList.GetItemData(selected);
		int item = m_LeftList.InsertString(-1, itemText.c_str());
		m_LeftList.SetItemData(item, itemData);
		m_RightList.DeleteString(selected);

		//C(&m_LeftList);
		m_LeftList.SetCurSel(item);

	}
}

void ExcelExportSettingsDlg::OnMoveToRight()
{
	int selected = m_LeftList.GetCurSel();

	if (selected >= 0)
	{		
		std::wstring itemText = GetItemText(m_LeftList,selected);	
		DWORD itemData = m_LeftList.GetItemData(selected);
		int item = m_RightList.InsertString(-1, itemText.c_str());
		m_RightList.SetItemData(item, itemData);
		m_LeftList.DeleteString(selected);

		//SetFocus(&m_RightList);
		m_RightList.SetCurSel(item);

	}
}


std::wstring ExcelExportSettingsDlg::GetItemText(CListBox& list, int item)
{
	std::wstring result;
	int length = list.GetTextLen(item);
	wchar_t* itemText = new wchar_t[length + 1];
	list.GetText(item, itemText);
	result = itemText;
	delete itemText;
	return result;
}

void ExcelExportSettingsDlg::MoveItemDown(int nItem)
{
	MoveItem(nItem, nItem + 1);

}

void ExcelExportSettingsDlg::MoveItemUp(int nItem)
{
	MoveItem(nItem, nItem - 1);
}


void ExcelExportSettingsDlg::MoveItem(int nSrcItemPos, int nDstItemPos)
{
	int iItem;
	int nCount = m_RightList.GetCount();
	if (nSrcItemPos < nCount && nSrcItemPos >= 0 && nDstItemPos < nCount)
	{
		std::wstring itemText = GetItemText(m_RightList, nSrcItemPos);
		DWORD itemData = m_RightList.GetItemData(nSrcItemPos);
		m_RightList.DeleteString(nSrcItemPos);
		iItem = m_RightList.InsertString(nDstItemPos, itemText.c_str());
		m_RightList.SetItemData(iItem, itemData);
		m_RightList.SetCurSel(iItem);
	}
}