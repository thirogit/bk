#include "stdafx.h"
#include "BaseCombo.h"
#include <data\IIdable.h>
#include <arrays\DeepDelete.h>

ComboItem::ComboItem(uint32_t itemId,const std::wstring& itemText) : m_itemId(itemId),m_itemText(itemText)
{
}

ComboItem::ComboItem(const ComboItem& src)
{
	CopyFrom(src);
}

void ComboItem::CopyFrom(const ComboItem& src)
{
	m_itemId = src.m_itemId;
	m_itemText = src.m_itemText;
}

ComboItem& ComboItem::operator=(const ComboItem& right)
{
	CopyFrom(right);
	return *this;
}

uint32_t ComboItem::GetItemId() const
{
	return m_itemId;
}

const std::wstring& ComboItem::GetItemText() const
{
	return m_itemText;
}
//-------------------------------------------------------------------------------------------------

ComboContent::ComboContent()
{
}

ComboContent::~ComboContent()
{
	Destroy();
}

ComboContent::ComboContent(const ComboContent& src)
{
	CopyFrom(src);
}

void ComboContent::CopyFrom(const ComboContent& src)
{
	DeepDelete(m_Items);
	m_Items.clear();

	std::vector<ComboItem*>::const_iterator itemIt = src.m_Items.cbegin();
	while(itemIt != src.m_Items.cend())
	{
		m_Items.push_back(new ComboItem(**itemIt));
		itemIt++;
	}	
}

void ComboContent::AddItem(const ComboItem& item)
{
	m_Items.push_back(new ComboItem(item));
}

void ComboContent::AddItem(uint32_t itemId,const std::wstring& itemText)
{
	m_Items.push_back(new ComboItem(itemId,itemText));
}

ComboContent& ComboContent::operator()(uint32_t itemId,const std::wstring& itemText)
{
	AddItem(itemId,itemText);
	return *this;
}

PtrEnumerator<ComboItem> ComboContent::EnumItems()
{
	return PtrEnumerator<ComboItem>(m_Items);
}

ComboContent& ComboContent::operator=(const ComboContent& right)
{
	CopyFrom(right);
	return *this;
}

void ComboContent::Destroy()
{
	DeepDelete(m_Items);
	m_Items.clear();
}

//-------------------------------------------------------------------------------------------------
void CBaseCombo::Select(uint32_t itemId)
{
	for(int item = 0,count = GetCount();item < count;item++)
	{
		if(((uint32_t)GetItemData(item)) == itemId)
		{
			SetCurSel(item);
			return;
		}
	}
	SetCurSel(-1);	
}
	
void CBaseCombo::SetContent(ComboContent& content)
{
	int itemIndex;
	std::string itemText;
	ComboItem* pItem = NULL;
	ResetContent();	

	PtrEnumerator<ComboItem> itemEnum = content.EnumItems();
	while(itemEnum.hasNext())
	{
		pItem = *itemEnum;
		//itemIndex = (int)::SendMessageW(m_hWnd, CB_ADDSTRING, 0, (LPARAM)pItem->GetItemText().c_str());
		itemIndex = AddString(pItem->GetItemText().c_str());
		SetItemData(itemIndex,(DWORD_PTR)pItem->GetItemId());		
		itemEnum.advance();
	}	
}
	
uint32_t CBaseCombo::GetSelectedItemId()
{
	int nSelectedItem = GetCurSel();
	if(nSelectedItem < 0) return NULL_ID;
	return (uint32_t)GetItemData(nSelectedItem);
}
