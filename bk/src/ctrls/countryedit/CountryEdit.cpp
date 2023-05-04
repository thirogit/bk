#include "stdafx.h"
#include "CountryEdit.h"
#include <data\types\countries\Countries.h>
#include <compare\TypesCompare.h>
#include <vector>
#include "../../text/CountryNameText.h"
#include "../../resource.h"
#include <string/TextUtils.h>

#define DEFAULT_COUNTRY_ISO_NUM 616
#define IDM_COUNTRY_MIN (WM_APP+1)
#define IDM_COUNTRY_MAX (WM_APP+300)

BEGIN_MESSAGE_MAP(CCountryEdit, CEditWithBtnCtrl)
    ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(IDM_COUNTRY_MIN,IDM_COUNTRY_MAX,OnCountry)
	
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()	
END_MESSAGE_MAP()




CCountryEdit::CCountryEdit() : CEditWithBtnCtrl(),m_checkedCountry(DEFAULT_COUNTRY_ISO_NUM)
{
}

void CCountryEdit::OnCountryChange(const Country& country)
{
	SetButtonImage(m_CountryFlags.GetFlagByCountryNumber(country.GetCountryISONumber()));
	m_checkedCountry = country.GetCountryISONumber();
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW/* | RDW_ERASE*/);
}

void CCountryEdit::PreSubclassWindow()
{
	CEditWithBtnCtrl::PreSubclassWindow();
	SetButtonImage(m_CountryFlags.GetFlagByCountryNumber(m_checkedCountry));
	CreateCountryMenu();
}

void CCountryEdit::CreateCountryMenu()
{
	m_CountryMenu.CreatePopupMenu();
	
	COLORREF clrMenu = GetSysColor(COLOR_MENU); 

	MENUITEMINFO menuItem;
	
	int id=IDM_COUNTRY_MIN;
	
	std::vector<const Country*> countries;

	Countries::Enumerator countryEnum = Countries::EnumCountries();
	while(countryEnum.hasNext())
	{
		countries.push_back(*countryEnum++);	
	}

	std::function<bool (const Country*,const Country*)> comparator = 
		[](const Country* country1,const Country* country2) 
		{ 
			return TypesCompare::CompareLocaleString(CountryNameText(*country1).ToString(),CountryNameText(*country2).ToString()) != COMPARE_LESS; 
		};

	std::sort(countries.begin(),countries.end(), comparator );
	
	wchar_t szItemTextBuffer[100];

	std::vector<const Country*>::iterator countryIt = countries.begin();
	const Country* pCountry;
	while(countries.end() != countryIt)
	{
		pCountry = (*countryIt);
		HBITMAP hFlagBmp = m_CountryFlags.GetFlagBitmap(pCountry->GetCountryISONumber(),clrMenu);
		
		memset(&menuItem,0,sizeof(MENUITEMINFO));
		menuItem.cbSize = sizeof(MENUITEMINFO);
		menuItem.fMask =  MIIM_STRING | MIIM_BITMAP | MIIM_DATA | MIIM_ID;
	
		std::wstring itemText = TextUtils::Format(L"%s, %s", CountryNameText(*pCountry).ToString(), pCountry->GetCountryCode3A());
		wcscpy_s(szItemTextBuffer, itemText.c_str());
		menuItem.dwTypeData = szItemTextBuffer;
		menuItem.cch = wcslen(szItemTextBuffer);
		menuItem.hbmpItem = hFlagBmp;
		menuItem.dwItemData = pCountry->GetCountryISONumber();
		menuItem.wID = id;
		
		m_CountryMenu.InsertMenuItem(id,&menuItem);
		id++;
		countryIt++;
	}
}

void CCountryEdit::OnBtnClick()
{
	CRect windowRect;
	GetWindowRect(&windowRect);

	MENUITEMINFO menuItem;
	memset(&menuItem,0,sizeof(MENUITEMINFO));
	for(int itemPos = 0,itemCount = m_CountryMenu.GetMenuItemCount();itemPos < itemCount;itemPos++)
	{
		memset(&menuItem,0,sizeof(MENUITEMINFO));
		menuItem.cbSize = sizeof(MENUITEMINFO);
		menuItem.fMask =  MIIM_DATA;
		m_CountryMenu.GetMenuItemInfo(itemPos,&menuItem,TRUE);
		
		menuItem.fMask =  MIIM_STATE;
		menuItem.fState = m_checkedCountry == menuItem.dwItemData ? MFS_CHECKED : 0;
		m_CountryMenu.SetMenuItemInfo(itemPos,&menuItem,TRUE);	
		
	}


	m_CountryMenu.TrackPopupMenu(0,windowRect.left,windowRect.bottom,this);
}

const Country CCountryEdit::GetCountry(unsigned int countryISONumber)
{
	return Countries::GetCountryByNumber(countryISONumber);	
}

const Country CCountryEdit::GetCountry(const std::wstring& countryCode2a)
{
	return Countries::GetCountryByCode2a(countryCode2a);	
}

void CCountryEdit::SetCountry(const Country& country)
{
	OnCountryChange(country);
}

void CCountryEdit::OnCountry(UINT id)
{
	MENUITEMINFO menuItem;
	memset(&menuItem,0,sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
	menuItem.fMask = MIIM_DATA;
	if(m_CountryMenu.GetMenuItemInfo(id,&menuItem))
	{
		unsigned int selectedCountryNum = menuItem.dwItemData;
		if(selectedCountryNum != m_checkedCountry)
		{
			m_checkedCountry = selectedCountryNum;
			OnCountryChange(Countries::GetCountryByNumber(m_checkedCountry));
		}
	}
}