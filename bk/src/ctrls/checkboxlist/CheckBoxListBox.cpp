#include "stdafx.h"
#include "CheckBoxListBox.h"

BEGIN_MESSAGE_MAP(CCheckBoxListBox, CCheckListBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CCheckBoxListBox::CCheckBoxListBox()
{
}

CCheckBoxListBox::~CCheckBoxListBox()
{
}


void CCheckBoxListBox::AddOption(uint32_t optionId,const std::wstring& sOptDesc,bool bValue)
{
	CheckBoxItem* item = new CheckBoxItem(optionId,sOptDesc,bValue);
	int iItem = InsertString(-1,item->GetOptionDesc().c_str());
	SetCheck(iItem, item->GetOptionValue());
	SetItemData(iItem,(DWORD_PTR)item);
}

void CCheckBoxListBox::UpdateOptions()
{
	CheckBoxItem* item = NULL;
	for(int i = 0,count = GetCount();i < count;i++)
	{
		item = (CheckBoxItem*)GetItemData(i);
		item->SetOptionValue(GetCheck(i) == 1);
	}
}

void CCheckBoxListBox::MoveItemDown(int nItem)
{
	MoveItem(nItem,nItem+1);
	
}

void CCheckBoxListBox::MoveItemUp(int nItem)
{
	MoveItem(nItem,nItem-1);
}


void CCheckBoxListBox::MoveItem(int nSrcItemPos,int nDstItemPos)
{
	int iItem;
	int nCount = this->GetCount();
	if(nSrcItemPos < nCount && nSrcItemPos >= 0 && nDstItemPos < nCount)
	{
		CheckBoxItem* item = (CheckBoxItem*)GetItemData(nSrcItemPos);
		DeleteString(nSrcItemPos);
		iItem = InsertString(nDstItemPos, item->GetOptionDesc().c_str());
		SetCheck(iItem, item->GetOptionValue());
		SetItemData(iItem,(DWORD_PTR)item);
		SetCurSel(iItem);
	}
}

void CCheckBoxListBox::OnDestroy()
{
	CheckBoxItem* item = NULL;
	for(int i = 0,count = GetCount();i < count;i++)
	{
		item = (CheckBoxItem*)GetItemData(i);
		delete item;
	}

}