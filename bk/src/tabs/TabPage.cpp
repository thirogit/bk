#include "stdafx.h"
#include "TabPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TabPage_CLASSNAME    _T("TabPage")  

TabPage::TabPage()
{
	RegisterWindowClass();
}

TabPage::~TabPage()
{
}


BEGIN_MESSAGE_MAP(TabPage, CWnd)
	ON_WM_CREATE()

	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL TabPage::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, TabPage_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = TabPage_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL TabPage::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}

void TabPage::OnSize( UINT nType,  int cx,  int cy  )
{
	CWnd::OnSize(nType,cx,cy);

	

}

BOOL TabPage::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	return CWnd::Create(TabPage_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}


int TabPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) != 0)
		return -1;
	

	//if(!m_HerdsTab.Create(WS_CHILD| WS_VISIBLE | CTCS_LEFT,CRect(),this,ID_HERDS_TAB_ID))
	//	return -1;
	
	//m_HerdsTab.InsertItem(0, "Tab 1");
	//m_HerdsTab.InsertItem(1, "Tab 2");
	//m_HerdsTab.InsertItem(2, "Tab 3");
	//m_HerdsTab.InsertItem(3, "Tab 4");
	

	return 0;
}