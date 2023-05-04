#include "stdafx.h"
#include "VNavigation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_ID_MIN 1001
#define TAB_GAP 12

BEGIN_MESSAGE_MAP(CVNavigation::Tab, CStatic)

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	
END_MESSAGE_MAP()


CVNavigation::Tab::Tab(CVNavigation* parent) : m_parent(parent), m_bLBtnDown(FALSE)
{
	m_FgBrush.CreateSolidBrush(RGB(127,127,127));
	m_HotBrush.CreateSolidBrush(RGB(28, 151, 234));
	m_SelectedBrush.CreateSolidBrush(RGB(0, 122, 204));
}
	

void CVNavigation::Tab::SetLook(const TabLook* look)
{
	m_FgBrush.DeleteObject();
	m_FgBrush.CreateSolidBrush(look->notactive);

	m_HotBrush.DeleteObject();
	m_HotBrush.CreateSolidBrush(look->hot);

	m_SelectedBrush.DeleteObject();
	m_SelectedBrush.CreateSolidBrush(look->active);
}

BOOL CVNavigation::Tab::Create(const std::wstring& caption, const CRect& rect, CWnd* pParentWnd, UINT id)
{
	m_bMouseTracking = FALSE;
	m_bMouseOver = FALSE;
	return CStatic::Create(caption.c_str(), WS_VISIBLE | WS_CHILD | WS_TABSTOP | SS_NOTIFY, rect, pParentWnd, id);
}

void CVNavigation::Tab::OnPaint()
{
	const int barHeight = 6;
	CPaintDC dc(this); // device context for painting

	dc.SaveDC();

	bool bSelected = m_parent->IsSelected(this);

	CFont* pFont = dc.SelectObject(bSelected ? &m_parent->m_SelectedFont : &m_parent->m_Font);
	CRect rectClient;
	GetClientRect(rectClient);

	CBrush* brush = &m_FgBrush;

	if (bSelected)
		brush = &m_SelectedBrush;
	else if(m_bMouseOver == TRUE)
		brush = &m_HotBrush;

	dc.FillRect(CRect(0, 0, barHeight, rectClient.Height()), brush);

	int captionBufferSize = GetWindowTextLength() +1;
	wchar_t* caption = new wchar_t[captionBufferSize];
	GetWindowText(caption, captionBufferSize);

	

	CRect txtRect(rectClient);// 0, 0, rectClient.Height(), rectClient.Width() - barHeight);
	SetGraphicsMode(dc.GetSafeHdc(), GM_ADVANCED);

	XFORM form;
	double angle = -90 * 3.14159 / 180.0;
	double s = sin(angle);
	double c = cos(angle);

	form.eM11 = c;
	form.eM12 = s;
	form.eM21 = -s;
	form.eM22 = c;

	form.eDx = 0;
	form.eDy = 0;

	dc.SetTextAlign(TA_LEFT | TA_TOP);
	SetWorldTransform(dc.GetSafeHdc(), &form);
	dc.DPtoLP(&txtRect);
		
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(caption, txtRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	delete[] caption;

	dc.SelectObject(pFont);


	dc.RestoreDC(-1);
}

BOOL CVNavigation::Tab::OnEraseBkgnd(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);
	pDC->FillRect(&cr, m_parent->m_pBkgBrush);

	return TRUE;
}

void CVNavigation::Tab::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}

void CVNavigation::Tab::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLBtnDown = TRUE;
}

void CVNavigation::Tab::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bLBtnDown)
	{
		m_parent->SelectTab(this);
		m_bLBtnDown = FALSE;
	}

}

void CVNavigation::Tab::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking)
	{

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = this->m_hWnd;

		if (::_TrackMouseEvent(&tme))
		{

			m_bMouseTracking = TRUE;		
			m_bMouseOver = TRUE;
			Invalidate(FALSE);
		}

	}
}


LRESULT CVNavigation::Tab::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{

	m_bMouseTracking = FALSE;
	m_bMouseOver = FALSE;
	Invalidate(FALSE);
	return TRUE;

}

LRESULT CVNavigation::Tab::OnMouseHover(WPARAM wParam, LPARAM lParam)
{	
	m_bMouseOver = TRUE;
	return TRUE;

}


BEGIN_MESSAGE_MAP(CVNavigation, CStatic)
	
	//ON_WM_PAINT()
	//ON_WM_ERASEBKGND()	
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


CVNavigation::CVNavigation() : m_pBkgBrush(NULL), m_idSequence(TAB_ID_MIN), m_pSelectedTab(NULL)
{
	
	const wchar_t* faceName = L"Arial";
	const int fontHeight = -16;

	m_pBkgBrush = new CBrush(GetSysColor(COLOR_BTNFACE));
	
	
	m_Font.CreateFont(fontHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, faceName);
	m_SelectedFont.CreateFont(fontHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, faceName);

	
}

void CVNavigation::SelectTab(Tab* tab)
{
	m_pSelectedTab = tab;
	Invalidate(FALSE);
	if (m_pSelectedTab->m_onSelected)
	{
		m_pSelectedTab->m_onSelected(m_pSelectedTab->GetDlgCtrlID());
	}
}

bool CVNavigation::IsSelected(Tab* tab)
{
	return tab == m_pSelectedTab;
}

int CVNavigation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(!CStatic::OnCreate(lpCreateStruct))
		return 1;

	CreateObjects();
	

	return 0;
}



CVNavigation::~CVNavigation()
{
	

	if (m_pBkgBrush)
	{
		if(m_pBkgBrush->GetSafeHandle())
			m_pBkgBrush->DeleteObject();
	}	
	delete m_pBkgBrush;

	m_Font.DeleteObject();
	m_SelectedFont.DeleteObject();
	
	
}
void CVNavigation::DestroyObjects()
{
	auto it = m_tabs.begin();
	while (it != m_tabs.end())
	{
		Tab* tab = *it;
		tab->DestroyWindow();
		delete tab;
		it++;		
	}	
}

void  CVNavigation::CreateObjects()
{

}

void CVNavigation::OnDestroy()
{
	DestroyObjects();
	CStatic::OnDestroy();
}

void CVNavigation::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
	ModifyStyleEx(GetExStyle(), WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP | SS_NOTIFY);
	CreateObjects();	
}

void CVNavigation::OnPaint() 
{
	
}


int CVNavigation::CalculateHeight()
{
	int h = 0;
	auto it = m_tabs.begin();
	while (it != m_tabs.end())
	{
		Tab* tab = *it;

		CRect rect;
		tab->GetClientRect(&rect);

		h += rect.Height();

		it++;

		if (it != m_tabs.end())
		{
			h += TAB_GAP;
		}
	}


	
	return h;
}

BOOL CVNavigation::OnEraseBkgnd(CDC* pDC) 
{
	CRect cr;
	GetClientRect(cr); 
	pDC->FillRect(&cr, m_pBkgBrush);

	return TRUE; 
}

UINT CVNavigation::AddTab(const std::wstring& tabText, std::function<void(UINT)>& onSelected, const TabLook* look/* = NULL*/)
{
	CClientDC dc(this);

	dc.SelectObject(&m_SelectedFont);
	CSize size = dc.GetTextExtent(tabText.c_str(), tabText.length());

	CRect rect;
	GetClientRect(&rect);

	Tab* tab = new Tab(this);	

	int h = CalculateHeight();	
	int top = h + (h > 0 ? TAB_GAP : 0);
	tab->Create(tabText, CRect(0, top, rect.Width(), top+size.cx+6), this,++m_idSequence);
	if (look != NULL)
	{
		tab->SetLook(look);
	}

	m_tabs.push_back(tab);

	return m_idSequence;
	
}

