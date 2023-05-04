#include "stdafx.h"
#include "MenuBuilder.h"
#include <arrays\DeepDelete.h>
#include <iterator>

IdSequence::IdSequence(uint32_t start) : m_value(start)
{
}

uint32_t IdSequence::next()
{
	return ++m_value;
}

uint32_t IdSequence::current() const
{
	return m_value;
}

//-------------------------------------------------------------------------------------------------
MenuBuilder::MenuItem::MenuItem() : type(ItemType_None),submenu(NULL)
{
}

MenuBuilder::MenuItem::MenuItem(const MenuBuilder::MenuItem& src) : type(src.type),handler(src.handler),text(src.text),submenu(NULL)
{
	if(src.submenu)
		submenu = new MenuBuilder(*src.submenu);
}

MenuBuilder::MenuItem::~MenuItem()
{
	if(submenu)
		delete submenu;
}
	

MenuBuilder::MenuBuilder()
{
}

MenuBuilder::MenuBuilder(const MenuBuilder& src)
{
	MenuItem* item;
	auto itemIt = src.m_items.begin();
	while(itemIt != src.m_items.end())
	{
		item = *itemIt;
		m_items.push_back(new MenuItem(*item));
		itemIt++;
	}
}

MenuBuilder::MenuBuilder(MenuBuilder&& src)
{
	std::copy(src.m_items.begin(),src.m_items.end(),std::back_inserter(m_items));
	src.m_items.clear();
}

MenuBuilder::~MenuBuilder()
{
	DeepDelete(m_items);
}

MenuBuilder& MenuBuilder::add(const std::wstring& sItemText,std::function<void ()>& handler)
{
	MenuBuilder::MenuItem* item = new MenuBuilder::MenuItem();
	item->type = ItemType_Text;
	item->text = sItemText;
	item->handler = handler;
	m_items.push_back(item);
	return *this;
}

MenuBuilder& MenuBuilder::addSeparator()
{
	MenuBuilder::MenuItem* item = new MenuBuilder::MenuItem();
	item->type = ItemType_Separator;	
	m_items.push_back(item);
	return *this;
}

MenuBuilder& MenuBuilder::addSubmenu(const std::wstring& sItemText,const MenuBuilder& submenu)
{
	MenuBuilder::MenuItem* item = new MenuBuilder::MenuItem();
	item->type = ItemType_Submenu;	
	item->text = sItemText;
	item->submenu = new MenuBuilder(submenu);
	m_items.push_back(item);
	return *this;
}

HMENU MenuBuilder::createMenu(IdSequence& idSeq)
{
	CMenu menu;
	menu.CreatePopupMenu();

	MENUITEMINFO menuItem;
	MenuItem* item;
	auto itemIt = m_items.begin();
	while(itemIt != m_items.end())
	{
		item = *itemIt;
		memset(&menuItem,0,sizeof(MENUITEMINFO));
		menuItem.cbSize = sizeof(MENUITEMINFO);
		
		switch(item->type)
		{
		case ItemType_Separator:
			menuItem.fMask =  MIIM_TYPE;
			menuItem.fType = MFT_SEPARATOR;
			break;
		case ItemType_Text:
			menuItem.fMask =  MIIM_STRING | MIIM_ID | MIIM_DATA ;
			menuItem.dwTypeData = const_cast<wchar_t*>(item->text.c_str());
			menuItem.cch = item->text.length();
			menuItem.wID = idSeq.next();
			menuItem.dwItemData = (ULONG_PTR)item;
			break;
		case ItemType_Submenu:
			menuItem.fMask =  MIIM_SUBMENU | MIIM_STRING | MIIM_ID ;
			menuItem.dwTypeData = const_cast<wchar_t*>(item->text.c_str());
			menuItem.cch = item->text.length();
			menuItem.wID = idSeq.next();
			menuItem.hSubMenu = item->submenu->createMenu(idSeq);
			break;
		}

		menu.InsertMenuItem(menu.GetMenuItemCount(),&menuItem,TRUE);
		itemIt++;
	}

	
	return menu.Detach();
}
	
uint32_t MenuBuilder::trackAt(int x,int y,CWnd* pWnd)
{

	IdSequence seq(1000);
	HMENU menu = createMenu(seq);	
	uint32_t selectedId = ::TrackPopupMenu(menu,  TPM_RETURNCMD | TPM_NONOTIFY, x, y, 0, pWnd->GetSafeHwnd(),NULL);

	if(selectedId != 0)
	{
		MENUITEMINFO menuItem;
		memset(&menuItem,0,sizeof(MENUITEMINFO));
		menuItem.cbSize = sizeof(MENUITEMINFO);
		menuItem.fMask = MIIM_DATA ;
		
		::GetMenuItemInfo(menu,selectedId,FALSE,&menuItem);

		MenuItem* item = (MenuItem*)menuItem.dwItemData;
		if(item->handler)
			item->handler();
	}

	DestroyMenu(menu);

	return 0;
}

uint32_t MenuBuilder::track(CWnd* pWnd)
{
	CPoint point;
	GetCursorPos(&point);
	return trackAt(point.x,point.y,pWnd);
}

size_t MenuBuilder::itemCount() const
{
	return m_items.size();
}
