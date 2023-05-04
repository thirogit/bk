#include "stdafx.h"
#include "HentsTab.h"
#include "../../compare\HentComparator.h"
#include "../../resource.h"
#include "../../utils/StringLoader.h"
#include "../../SeasonAccess.h"
#include "../../image/Png.h"
#include "../../hent/HentDlg.h"
#include <boost\bind.hpp>
#include <future>
#include <utility>
#include <functional>
#include "../../view/HentViewCreator.h"
#include <arrays\DeepDelete.h>
#include "../../uiconstants/ColorConstants.h"
#include "../../hent/HentSearchDlg.h"
#include "../../ui/menu/MenuBuilder.h"
#include "../../ui/ExceptionMsgBox.h"
#include "../../content/compare/RowComparator.h"
#include "../../compare/HentComparator.h"
#include "../../compare/DataComparator.h"
#include "../../ui/YesNoBox.h"
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include "../find/FindHentDlg.h"

namespace ph = std::placeholders;

HentsListContent::HentsListContent() 
{
}

HentsListContent::~HentsListContent()
{	
}

void  HentsListContent::BeforeHentChange()
{
	NotifyBeforeUpdate();
}

void  HentsListContent::AfterHentChange()
{
	NotifyAfterUpdate();
}

void  HentsListContent::OnHentInserted(uint32_t hentId) 
{
	HentClientObject* pHentCO = m_pSession->GetHent(hentId);
	HentViewCreator viewCreator(m_pSession);
	HentRow* pRow = new HentRow(viewCreator.CreateView(pHentCO));
	m_Rows.push_back(pRow);
	m_RowToHentId[pHentCO->Object()->GetId()] = pRow;	

	NotifyCountChanged();
}

void  HentsListContent::OnHentLocked(uint32_t hentId)
{
	RefreshHentsRow(hentId);
}

void HentsListContent::RefreshHentsRow(uint32_t hentId)
{
	auto rowIt = m_RowToHentId.find(hentId);
	if(rowIt != m_RowToHentId.end())
	{
		HentRow* pRow = rowIt->second;
		HentViewCreator viewCreator(m_pSession);
		HentView* pView = pRow->Content();
		viewCreator.RefreshView(pView,m_pSession->GetHent(pView->GetHentId()));
		NotifyContentUpdated();		
	}
}

void  HentsListContent::OnHentUnlocked(uint32_t hentId)
{
	RefreshHentsRow(hentId);
}	

void  HentsListContent::OnHentUpdated(uint32_t hentId) 
{
	RefreshHentsRow(hentId);
}

void  HentsListContent::OnHentDeleted(uint32_t hentId) 
{
	auto rowIt = m_Rows.begin();
	auto endIt = m_Rows.end();
	HentRow* row;
	while(rowIt != endIt)
	{
		row = *rowIt;
		if(row->Content()->GetHentId() == hentId)
		{
			m_Rows.erase(rowIt);
			delete row;
			break;
		}
		rowIt++;
	}

	NotifyCountChanged();
}

void HentsListContent::SortFound()
{
	std::sort(m_Rows.begin(), m_Rows.end(), [](HentRow* r1, HentRow* r2)
	{
		return r1->IsMarked() < r2->IsMarked();
	});
}


void HentsListContent::CreateContent(SeasonSession* pSession)
{
	m_pSession = pSession;
			 
	PtrEnumerator<HentClientObject> hentCOEnum = m_pSession->EnumHents();
	HentViewCreator viewCreator(m_pSession);
	HentClientObject* pHentCO = NULL;
	HentRow* pRow = NULL;
	while(hentCOEnum.hasNext())
	{
		pHentCO = *hentCOEnum;
		pRow = new HentRow(viewCreator.CreateView(pHentCO));
		m_Rows.push_back(pRow);
		m_RowToHentId[pHentCO->Object()->GetId()] = pRow;
		hentCOEnum.advance();
	}
	m_pSession->AddSeasonObserver(this);		
}

void HentsListContent::DestroyContent()
{
	if(m_pSession)
		m_pSession->RemoveSeasonObserver(this);		

	DeepDelete(m_Rows);						
	m_Rows.clear();
	m_RowToHentId.clear();
	m_pSession = NULL;
}

int HentsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* HentsListContent::GetRow(int row)
{
	return GetHentAtRow(row);
}

HentRow* HentsListContent::GetHentAtRow(int row)
{
	return m_Rows[row];
}

void HentsListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<HentComparator> > comparator(attributes);
	std::sort(m_Rows.begin(), m_Rows.end(), comparator);
}

FindResult HentsListContent::FindHents(Predicate<HentView>* predicate)
{
	FindResult result;
	result.count = 0;
	result.firstOccurence = -1;
	
	int i = 0;
	BOOST_FOREACH(HentRow* row, m_Rows)
	{
		row->SetMarked(false);
		if (predicate->Test(row->Content()))
		{
			result.count++;
			row->SetMarked(true);

			if(result.firstOccurence < 0)
			{ 
				result.firstOccurence = i;
			}
		}
		i++;
	}

	
	NotifyContentUpdated();
	


	 
	return result;
}

void HentsListContent::ClearFound()
{
	BOOST_FOREACH(HentRow* row, m_Rows)
	{
		row->SetMarked(false);
	}
	NotifyContentUpdated();
}
//-------------------------------------------------------------------------------------------------

HentsTab::HentsTab() : m_HdrLayout(L"HENTS"), m_FindContainer(NULL)
{

}

HentsTab::~HentsTab()
{
	if (m_FindContainer)
		delete m_FindContainer;
}


BEGIN_MESSAGE_MAP(HentsTab, ListTab)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SHOWFIND, OnShowFind)
	ON_WM_SIZE()	
	ON_MESSAGE(WM_FIND, OnFind)
	ON_MESSAGE(WM_HIDEFIND, OnHideFind)
	ON_MESSAGE(WM_CLEARFOUND, OnClearFound)
	ON_MESSAGE(WM_SORTFOUND, OnSortFound)
END_MESSAGE_MAP()


int HentsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(ListTab::OnCreate(lpCreateStruct) != 0)
		return -1;	
	
	ListHeaderDescriptor hentsHeader;
	
	hentsHeader (HentViewMember::Name,					StringLoader(IDS_HEADER_HENTNAME))
				(HentViewMember::HentNo,				StringLoader(IDS_HEADER_HENTNO))
				(HentViewMember::Alias,					StringLoader(IDS_HEADER_HENTALIAS))
				(HentViewMember::Street,				StringLoader(IDS_HEADER_STREET))
				(HentViewMember::POBox,					StringLoader(IDS_HEADER_POBOX))
				(HentViewMember::City,					StringLoader(IDS_HEADER_CITY))
				(HentViewMember::Zip,					StringLoader(IDS_HEADER_ZIP))
				(HentViewMember::PlateNo,				StringLoader(IDS_HEADER_PLATENO))
				(HentViewMember::Type,					StringLoader(IDS_HEADER_HENTTYPE))
				(HentViewMember::SyncIt,				StringLoader(IDS_HEADER_SYNCIT))
				(HentViewMember::Extras,				StringLoader(IDS_HEADER_EXTRAS))
				(HentViewMember::PhoneNo,				StringLoader(IDS_HEADER_PHONENO))
				(HentViewMember::FiscalNo,				StringLoader(IDS_HEADER_FISCALNO))
				(HentViewMember::WetNo,					StringLoader(IDS_HEADER_WETNO))
				(HentViewMember::PersonalNo,			StringLoader(IDS_HEADER_PERSONALNO))
				(HentViewMember::StatNo,				StringLoader(IDS_HEADER_STATNO))
				(HentViewMember::PersonalIdNo,			StringLoader(IDS_HEADER_PERSONALIDNO))
				(HentViewMember::PersonalIdIssueDt,		StringLoader(IDS_HEADER_PERSIDISSUEDT))
				(HentViewMember::PersonalIdIssuePost,	StringLoader(IDS_HEADER_PERSIDISSUEPORT))
				(HentViewMember::AccountNo,				StringLoader(IDS_HEADER_ACCOUNTNO))
				(HentViewMember::BankName,				StringLoader(IDS_HEADER_BANKNAME))
				(HentViewMember::WetLicenceNo,			StringLoader(IDS_HEADER_WETLICENCENO))
				(HentViewMember::CellPhoneNo,			StringLoader(IDS_HEADER_CELLPHONENO))
				(HentViewMember::EmailAddress,			StringLoader(IDS_HEADER_EMAILADDRESS))
				(HentViewMember::Latitude,				StringLoader(IDS_HEADER_LATITUDE))
				(HentViewMember::Longitude,				StringLoader(IDS_HEADER_LONGITUDE));

	SetHeaderDescriptor(hentsHeader);

	SeasonSession* session = GetSeasonSession();

	m_HdrLayout.Load(session);		
	m_ListContent.CreateContent(session);
	SetContent(&m_ListContent);
	SetHeaderLayout(&m_HdrLayout);

	
	std::function<void (int,int)> dblClick = [this](int row,int col)
											 {														
												 OnEditHent();
											 };
	SetOnDblClick(dblClick);

	SetOnRightClick(std::bind(&HentsTab::OnRClick,this,ph::_1,ph::_2));

	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TABACCELERATOR));
		
	m_FindContainer = new FindContainerDlg(RUNTIME_CLASS(FindHentDlg), this);
	m_FindContainer->Create();
	m_bFindVisible = FALSE;

	return 0;
}

BOOL HentsTab::PreTranslateMessage(MSG* pMsg) 
{
	if (m_hAccelTable) {
		if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
			return(TRUE);
		}
	}
	return ListTab::PreTranslateMessage(pMsg);

}

void HentsTab::OnSize(UINT nType, int cx, int cy)
{
	TabPage::OnSize(nType, cx, cy);
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


LRESULT HentsTab::OnFind(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)FindHents((Predicate<HentView>*)lParam);
}


LRESULT HentsTab::OnHideFind(WPARAM wParam, LPARAM lParam)
{
	if (m_bFindVisible)
	{
		m_bFindVisible = FALSE;
		RecalcLayout();
		OnClearFound(0, 0);
	}
	return 0;
}

LRESULT HentsTab::OnClearFound(WPARAM wParam, LPARAM lParam)
{
	m_ListContent.ClearFound();
	return 0;
}

LRESULT HentsTab::OnSortFound(WPARAM wParam, LPARAM lParam)
{
	int rowCount = m_ListContent.GetRowCount();

	for (int i = 0; i < rowCount; i++)
	{
		HentRow* row = m_ListContent.GetHentAtRow(i);
		row->IsMarked();
	}
	return 0;
}

int HentsTab::FindHents(Predicate<HentView>* predicate)
{
	FindResult result = m_ListContent.FindHents(predicate);
	if (result.firstOccurence >= 0)
	{
		m_List.EnsureVisible(result.firstOccurence, FALSE);
	}

	return result.count;
}

void HentsTab::OnRClick(int row,int cow)
{	
	MenuBuilder menu;
	menu.add(StringLoader(IDS_ADD),std::bind(&HentsTab::OnAddHent,this));

	if (row >= 0)
	{
		menu.add(StringLoader(IDS_EDIT), std::bind(&HentsTab::OnEditHent, this));
		menu.add(StringLoader(IDS_DELETE), std::bind(&HentsTab::OnDeleteHent, this));
		menu.add(StringLoader(IDS_SEARCH), std::bind(&HentsTab::OnShowFind, this));

		MenuBuilder hentSyncMenu;
		hentSyncMenu.add(StringLoader(IDS_YES), std::bind(&HentsTab::OnHentSyncYes, this));
		hentSyncMenu.add(StringLoader(IDS_NO), std::bind(&HentsTab::OnHentSyncNo, this));		

		menu.addSubmenu(StringLoader(IDS_HENTSYNCED), hentSyncMenu);
	}

	menu.track(this);	
}


void HentsTab::OnHentSyncYes()
{
	
}

void HentsTab::OnHentSyncNo()
{

}

void HentsTab::OnShowFind()
{
	if (!m_bFindVisible)
	{
		m_bFindVisible = TRUE;
		RecalcLayout();
	}
}

void HentsTab::RecalcLayout()
{
	CRect rect;
	GetClientRect(&rect);
	OnSize(SIZE_RESTORED, rect.Width(), rect.Height());
}

void HentsTab::OnHideFind()
{
	if (m_bFindVisible)
	{
		m_bFindVisible = FALSE;
		RecalcLayout();
	}
}

void HentsTab::OnDestroy()
{
	m_HdrLayout.Save(GetSeasonSession());
	m_FindContainer->DestroyWindow();
	ListTab::OnDestroy();	
	m_ListContent.DestroyContent();
}

void HentsTab::OnDeleteHent()
{
	SeasonSession* session = GetSeasonSession();
	
	HentRow* pHentRow = m_ListContent.GetHentAtRow(GetSelectedRow());
	HentView* view = pHentRow->Content();
	uint32_t hentId = view->GetHentId();
	ExceptionMsgBox msgBox;

	if (YesNoBox().Ask((boost::wformat(StringLoader(IDS_ASKDELETEHENT)) % view->GetAlias()).str()))
	{

		Try()
			.Prepare([session, hentId]
		{
			session->LockHent(hentId);
		})
			.Do([session, hentId]
		{
			session->DeleteHent(hentId);
		})
			.Fallback([session, hentId]
		{
			session->UnlockHent(hentId);
		})
			.Catch([&msgBox](wexception& e)
		{
			msgBox.Show(e);
		})
			.Run();

	}
}

void HentsTab::OnAddHent()
{
	HentEditor editor(GetSeasonSession());
	editor.New();
	HentDlg hendDlg(&editor,this);
	hendDlg.DoModal();	
}

void HentsTab::OnFindHent()
{
	HentSearchDlg hentSearchDlg(this);
	hentSearchDlg.FindHent();
}

void HentsTab::OnEditHent()
{	
	int rowIndex = GetSelectedRow();
	if (rowIndex < 0) return;

	HentRow* pHentRow = m_ListContent.GetHentAtRow(rowIndex);
	uint32_t hentId = pHentRow->Content()->GetHentId();
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox msgBox;
			
	Try()
	.Prepare([session,hentId]	
	{
		session->LockHent(hentId);
	})
	.Do([this,session,hentId]
	{
		HentEditor editor(session);
		editor.Edit(hentId);
		HentDlg hendDlg(&editor,this);
		hendDlg.DoModal();
	})
	.TearDown([session,hentId]
	{
		session->UnlockHent(hentId);					
	})
	.Catch([&msgBox](std::exception& e)
	{
		msgBox.Show(e);
	})
	.Run();
}