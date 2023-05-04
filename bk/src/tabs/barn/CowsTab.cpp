#include "stdafx.h"
#include "CowsTab.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"
#include <logic\herdregistry\HerdRegistry.h>
#include <utils/SafeGet.h>
#include "../../text/DocNoText.h"
#include "../../SeasonAccess.h"
#include "../find/FindCowDlg.h"
#include "../../header/CowPresenceListHeader.h"


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
	ON_WM_SIZE()
	ON_MESSAGE(WM_FIND,OnFind)
	ON_MESSAGE(WM_HIDEFIND,OnHideFind)
	ON_MESSAGE(WM_CLEARFOUND, OnClearFound)
	ON_MESSAGE(WM_SORTFOUND, OnSortFound)

END_MESSAGE_MAP()




int CowsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(ListTab::OnCreate(lpCreateStruct) != 0)
		return -1;
	
	CowPresenceListHeader cowsHeader;

	m_List.SetHeaderDescriptor(cowsHeader);
	
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TABACCELERATOR));



	m_FindContainer = new FindContainerDlg(RUNTIME_CLASS(FindCowDlg),this);
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
		m_FindContainer->SetFocus();
	}
	else
	{
		m_FindContainer->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		m_List.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


LRESULT CowsTab::OnFind(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)Find((Predicate<CowPresenceView>*)lParam);
}

int CowsTab::Find(Predicate<CowPresenceView>* predicate)
{
	CowsListContent* content = (CowsListContent*)this->GetContent();

	FindResult result = content->Find(predicate);
	if (result.firstOccurence >= 0)
	{
		m_List.EnsureVisible(result.firstOccurence, FALSE);
	}

	return result.count;
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

LRESULT CowsTab::OnHideFind(WPARAM wParam, LPARAM lParam)
{
	if (m_bFindVisible)
	{
		m_bFindVisible = FALSE;
		RecalcLayout();
		OnClearFound(0, 0);
	}
	return 0;
}

LRESULT CowsTab::OnClearFound(WPARAM wParam, LPARAM lParam)
{
	CowsListContent* content = (CowsListContent*)this->GetContent();
	content->ClearFound();
	return 0;
}

LRESULT CowsTab::OnSortFound(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
