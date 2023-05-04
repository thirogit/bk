#include "stdafx.h"
#include "CollectorServerStatusBarCtrl.h"
#include "CollectorStatusColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LOGFONT CCollectorServerStatusBarCtrl::lf_default = { 12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft Sans Serif" };

CCollectorServerStatusBarCtrl::CCollectorServerStatusBarCtrl()
{
	m_font.CreateFontIndirectW(&lf_default);

}

CCollectorServerStatusBarCtrl::~CCollectorServerStatusBarCtrl()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCollectorServerStatusBarCtrl, CStatic)
	
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	
END_MESSAGE_MAP()

BOOL CCollectorServerStatusBarCtrl::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CCollectorServerStatusBarCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect( &rcClient );

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcClient);
	dc.SelectClipRgn(&rgn);
	
	dc.FillSolidRect( &rcClient, GetSysColor(COLOR_3DFACE) );

	const int boxW = 10, boxH = 16, gap = 2;

	int xLeftMargin = gap;
	int yTopMargin = 0;
	int h = rcClient.Height();

	COLORREF clrOldText = dc.SetTextColor( RGB(0,0,0) );
	INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
	DWORD dwFlags = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	CFont* oldFont = dc.SelectObject(&m_font);

	if (h > boxH)
	{
		yTopMargin = (h - boxH) / 2;
	}

	CRect box(xLeftMargin, yTopMargin, xLeftMargin + boxW, yTopMargin + boxH);
	dc.FillSolidRect(&box, IDLE_COLOR);
	dc.DrawText(L"4", box,dwFlags);

	box.OffsetRect(boxW + gap,0);
	dc.FillSolidRect(&box, BUSY_COLOR);
	dc.DrawText(L"6", box,dwFlags);

	box.OffsetRect(boxW + gap,0);
	dc.FillSolidRect(&box, READY_COLOR);
	dc.DrawText(L"1", box,dwFlags);

	box.OffsetRect(boxW + gap,0);
	dc.FillSolidRect(&box, SYNCING_COLOR);
	
	dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText(L"+", box,dwFlags);
	

	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );
	dc.SetTextColor(clrOldText);
	dc.SelectObject(oldFont);
	
}
