#include "stdafx.h"
#include "App.h"
#include "ChildView.h"
#include "utils/StringLoader.h"
//#include "resource.h"

#define MAIN_TAB_HEIGHT 20
#define ID_MAIN_TAB_CTRL_ID 1001

#define ID_BARN_TAB_ID 1002
#define ID_HENTS_TAB_ID 1003
#define ID_INVOICES_TAB_ID 1004

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); 	
}

void CChildView::OnSize( UINT nType,  int cx,  int cy  )
{
	CWnd::OnSize(nType,cx,cy);

	m_MainTabCtrl.SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER | SWP_NOMOVE);

}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) != 0)
		return -1;
	

	if (!m_MainTabCtrl.Create(CMFCTabCtrl::STYLE_3D_VS2005, CRect(), this, ID_MAIN_TAB_CTRL_ID, CMFCTabCtrl::Location::LOCATION_TOP))
		return -1;

	//m_MainTabCtrl.SetSelectedColor(RGB(255,127,39));

	//m_MainTabCtrl.SetFont(GetFont());

	UINT tabStyle = WS_CHILD| WS_VISIBLE;
	CRect tabRect;

	m_BarnTab.Create(tabStyle ,tabRect,&m_MainTabCtrl,ID_BARN_TAB_ID);
	m_HentsTab.Create(tabStyle ,tabRect,&m_MainTabCtrl,ID_HENTS_TAB_ID);
	m_InvoicesTab.Create(tabStyle ,tabRect,&m_MainTabCtrl,ID_INVOICES_TAB_ID);

	m_MainTabCtrl.AddTab(&m_BarnTab,StringLoader(IDS_BARNTABNAME).c_str());
	m_MainTabCtrl.AddTab(&m_HentsTab, StringLoader(IDS_HENTSTABNAME).c_str());
	m_MainTabCtrl.AddTab(&m_InvoicesTab, StringLoader(IDS_INVOICESTABNAME).c_str());
	
	return 0;
}