#include "stdafx.h"
#include "CowsTab.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"
#include <logic\herdregistry\HerdRegistry.h>
#include <utils/SafeGet.h>
#include "../../text/DocNoText.h"
#include "../../SeasonAccess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_COWS_LIST 1001

CowsTab::CowsTab() 
{
}


BEGIN_MESSAGE_MAP(CowsTab, ListTab)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SHOWFIND, OnShowFind)
	ON_COMMAND(ID_HIDEFIND, OnHideFind)
	ON_WM_SIZE()
	ON_MESSAGE(WM_FIND,OnFind)
END_MESSAGE_MAP()




int CowsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(ListTab::OnCreate(lpCreateStruct) != 0)
		return -1;
	
	ListHeaderDescriptor cowsHeader;
	
	cowsHeader	(CowPresenceViewMember::State,			StringLoader(IDS_HEADER_STATE))
		(CowPresenceViewMember::CowNo,			StringLoader(IDS_HEADER_COWNO))
		(CowPresenceViewMember::PassNo,			StringLoader(IDS_HEADER_PASSNO))	
		(CowPresenceViewMember::FirstOwner,		StringLoader(IDS_HEADER_FSTOWNER))
		(CowPresenceViewMember::BirthDate,		StringLoader(IDS_HEADER_BIRTHDATE))
		(CowPresenceViewMember::Weight,			StringLoader(IDS_HEADER_WEIGHT))
		(CowPresenceViewMember::Sex,			StringLoader(IDS_HEADER_SEX))
		(CowPresenceViewMember::Stock,			StringLoader(IDS_HEADER_STOCK))
		(CowPresenceViewMember::Class,			StringLoader(IDS_HEADER_CLASS))
		(CowPresenceViewMember::Extras,			StringLoader(IDS_HEADER_EXTRAS))
		(CowPresenceViewMember::BuyFromHent,	StringLoader(IDS_HEADER_BUYFROMHENT))
		(CowPresenceViewMember::BuyStock,		StringLoader(IDS_HEADER_BUYSTOCK))
		(CowPresenceViewMember::BuyPrice,		StringLoader(IDS_HEADER_BUYPRICE))
		(CowPresenceViewMember::BuyWeight,		StringLoader(IDS_HEADER_BUYWEIGHT))
		(CowPresenceViewMember::HerdInDocNo,	StringLoader(IDS_HEADER_HERDINDOCNO))
		(CowPresenceViewMember::HerdInDate,		StringLoader(IDS_HEADER_HERDINDATE))
		(CowPresenceViewMember::HerdInGrp,		StringLoader(IDS_HEADER_HERDINGRP))
		(CowPresenceViewMember::HerdInClass,	StringLoader(IDS_HEADER_HERDINCLASS))
		(CowPresenceViewMember::HerdInWeight,	StringLoader(IDS_HEADER_HERDINWEIGHT))
		(CowPresenceViewMember::SellToHent,		StringLoader(IDS_HEADER_SELLTOHENT))
		(CowPresenceViewMember::SellStock,		StringLoader(IDS_HEADER_SELLSTOCK))
		(CowPresenceViewMember::SellPrice,		StringLoader(IDS_HEADER_SELLPRICE))
		(CowPresenceViewMember::SellWeight,		StringLoader(IDS_HEADER_SELLWEIGHT))
		(CowPresenceViewMember::HerdOutDocNo,	StringLoader(IDS_HEADER_HERDOUTDOCNO))
		(CowPresenceViewMember::HerdOutDate,	StringLoader(IDS_HEADER_HERDOUTDATE))
		(CowPresenceViewMember::HerdOutGrp,		StringLoader(IDS_HEADER_HERDOUTGRP))
		(CowPresenceViewMember::HerdOutClass,	StringLoader(IDS_HEADER_HERDOUTCLASS))
		(CowPresenceViewMember::HerdOutWeight,	StringLoader(IDS_HEADER_HERDOUTWEIGHT))
		(CowPresenceViewMember::BuyInvoiceNo,	StringLoader(IDS_HEADER_BUYINVOICE))
		(CowPresenceViewMember::SellInvoiceNo,	StringLoader(IDS_HEADER_SELLINVOICE))
		(CowPresenceViewMember::TermBuyClass,	StringLoader(IDS_HEADER_TERMBUYCLASS))
		(CowPresenceViewMember::TermSellClass,	StringLoader(IDS_HEADER_TERMSELLCLASS))
		(CowPresenceViewMember::TermBuyWeight,	StringLoader(IDS_HEADER_TERMBUYWEIGHT))
		(CowPresenceViewMember::TermSellWeight,	StringLoader(IDS_HEADER_TERMSELLWEIGHT))
		(CowPresenceViewMember::TermBuyPrice,	StringLoader(IDS_HEADER_TERMBUYPRICE))
		(CowPresenceViewMember::TermSellPrice,	StringLoader(IDS_HEADER_TERMSELLPRICE));

	m_List.SetHeaderDescriptor(cowsHeader);
	
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TABACCELERATOR));

	m_FindContainer = new FindContainerDlg(this);
	m_FindContainer->Create();
	m_bFindVisible = FALSE;

	return 0;
}

BOOL CowsTab::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccelTable) {
		if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
			return(TRUE);
		}
	}
	return ListTab::PreTranslateMessage(pMsg);
}

void CowsTab::OnSize(UINT nType, int cx, int cy)
{
	ListTab::OnSize(nType, cx, cy);
	if (m_bFindVisible)
	{
		CRect findDlgRect;
		m_FindContainer->GetWindowRect(&findDlgRect);
		int w = findDlgRect.Width();
		m_FindContainer->SetWindowPos(&m_List, 0, 0, w, cy, SWP_SHOWWINDOW);

		m_List.SetWindowPos(NULL, w, 0, cx - w, cy, SWP_NOZORDER);
	}
	else
	{
		m_List.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


LRESULT CowsTab::OnFind(WPARAM wParam, LPARAM lParam)
{
	return Find((Predicate<CowPresenceView>*)lParam);
}

int CowsTab::Find(Predicate<CowPresenceView>* predicate)
{
	/*FindResult result = m_ListContent.FindHents(predicate);
	if (result.firstOccurence >= 0)
	{
		m_List.EnsureVisible(result.firstOccurence, FALSE);
	}

	return result.count;*/
	return 0;
}


void CowsTab::OnShowFind()
{
	if (!m_bFindVisible)
	{
		m_bFindVisible = TRUE;
		RecalcLayout();
	}
}

void CowsTab::RecalcLayout()
{
	CRect rect;
	GetClientRect(&rect);
	OnSize(SIZE_RESTORED, rect.Width(), rect.Height());
}

void CowsTab::OnDestroy()
{
	m_FindContainer->DestroyWindow();
	ListTab::OnDestroy();
}

void CowsTab::OnHideFind()
{
	if (m_bFindVisible)
	{
		m_bFindVisible = FALSE;
		RecalcLayout();
	}
}