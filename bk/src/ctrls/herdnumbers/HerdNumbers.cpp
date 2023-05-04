#include "stdafx.h"
#include "HerdNumbers.h"

BEGIN_MESSAGE_MAP(CXColorStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CXColorStatic::CXColorStatic() : m_pFont(NULL),m_pBrush(NULL)
{
	m_rgbText       = GetSysColor(COLOR_BTNTEXT);
	m_rgbBackground = GetSysColor(COLOR_BTNFACE);
	m_pBrush        = new CBrush(m_rgbBackground);
	m_pFont         = new CFont();
}

int CXColorStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(!CStatic::OnCreate(lpCreateStruct))
		return 1;

	CreateObjects();
	return 0;
}

CXColorStatic::~CXColorStatic()
{
	DestroyObjects();
	delete m_pFont;	
	delete m_pBrush;	
}

void CXColorStatic::DestroyObjects()
{
	if (m_pFont)
	{
		if(m_pFont->GetSafeHandle())
			m_pFont->DeleteObject();
	}

	if (m_pBrush)
	{
		if(m_pBrush->GetSafeHandle())
			m_pBrush->DeleteObject();		
	}
	
}

void  CXColorStatic::CreateObjects()
{
	// get current font
	CFont* pFont = GetFont();
	if (!pFont)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
		if (hFont)
			pFont = CFont::FromHandle(hFont);
	}
	ASSERT(pFont);
	ASSERT(pFont->GetSafeHandle());

	if(m_pFont->GetSafeHandle())
	{
		m_pFont->DeleteObject();
	}
	
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_pFont->CreateFontIndirect(&lf);


	if(m_pBrush->GetSafeHandle())
	{
		m_pBrush->DeleteObject();
	}
	
	m_pBrush->CreateSolidBrush(m_rgbBackground);
}

void CXColorStatic::OnDestroy()
{
	
}

void CXColorStatic::PreSubclassWindow() 
{
	CreateObjects();	
}

void CXColorStatic::OnPaint() 
{
	CPaintDC dc(this); 
	
	dc.SaveDC();

	dc.SetTextColor(m_rgbText);
	dc.SetBkColor(m_rgbBackground);
	dc.SetBkMode(OPAQUE);
	dc.SelectObject(m_pBrush);

	CRect rect;
	GetClientRect(rect); 

		dc.SelectObject(m_pFont);

		// get static's text
		CString strText = _T("");
		GetWindowText(strText);

		UINT nFormat = 0;
		DWORD dwStyle = GetStyle();

		// set DrawText format from static style settings
		if (dwStyle & SS_CENTER)
			nFormat |= DT_CENTER;
		else if (dwStyle & SS_LEFT)
			nFormat |= DT_LEFT;
		else if (dwStyle & SS_RIGHT)
			nFormat |= DT_RIGHT;

		if (dwStyle & SS_CENTERIMAGE)	// vertical centering ==> single line only
			nFormat |= DT_VCENTER | DT_SINGLELINE;
		else
			nFormat |= DT_WORDBREAK;

		rect.left += m_nXMargin;
		rect.top  += m_nYMargin;
		dc.DrawText(strText, rect, nFormat);

	
	dc.RestoreDC(-1);
}

BOOL CXColorStatic::OnEraseBkgnd(CDC* pDC) 
{
	CRect cr;
	GetClientRect(cr); 
	pDC->FillRect(&cr, m_pBrush);

	return TRUE; 
}

