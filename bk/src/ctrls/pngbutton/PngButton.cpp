#include "stdafx.h"
#include "PngButton.h"


BEGIN_MESSAGE_MAP(CPngButton, CButton)

	ON_WM_ERASEBKGND()
	

END_MESSAGE_MAP()

CPngButton::CPngButton() 
{

}

CPngButton::~CPngButton()
{

}

void CPngButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
	CButton::PreSubclassWindow();
}

BOOL CPngButton::OnEraseBkgnd(CDC* pDC)
{
    CRect rectClient;        // Rectangle for entire client area
    GetClientRect(&rectClient);

    // Fill the rectangle
    //pDC->FillSolidRect(&rectClient, m_crColor);

	return TRUE;
}


void CPngButton::SetImage(UINT nResPNG)
{
	m_image.LoadPNG(nResPNG);
}

//const int g_ciArrowSizeX = 4 ;
//const int g_ciArrowSizeY = 2 ;
//const int g_ciArrowSizeX = 7;
//const int g_ciArrowSizeY = 4;

void CPngButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct);

	CDC*    pDC      = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT    state    = lpDrawItemStruct->itemState;
    CRect   rc    = lpDrawItemStruct->rcItem;
	CRect	rDraw = rc;

	//if (m_bPopupActive)
	//	state |= ODS_SELECTED|ODS_FOCUS;

	//******************************************************
	//**                  Draw Outer Edge
	//******************************************************
	UINT uFrameState = DFCS_BUTTONPUSH|DFCS_ADJUSTRECT;

	if (state & ODS_SELECTED)
		uFrameState |= DFCS_PUSHED;

	if (state & ODS_DISABLED)
		uFrameState |= DFCS_INACTIVE;

	pDC->DrawFrameControl(&rDraw,
						  DFC_BUTTON,
						  uFrameState);


	if (state & ODS_SELECTED)
		rDraw.OffsetRect(1,1);

	//******************************************************
	//**                     Draw Focus
	//******************************************************
	if (state & ODS_FOCUS)
    {
		RECT rFocus = {rDraw.left,
					   rDraw.top,
					   rDraw.right - 1,
					   rDraw.bottom};

        pDC->DrawFocusRect(&rFocus);
    }

	rDraw.DeflateRect(::GetSystemMetrics(SM_CXEDGE),
					  ::GetSystemMetrics(SM_CYEDGE));

	
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	DrawImage(graphics, rDraw);	
}

void CPngButton::DrawImage(Gdiplus::Graphics& graphics, const CRect& drawRect)
{
	CPoint center = drawRect.CenterPoint();
	CSize size = m_image.GetSize();
	center.Offset(-size.cx / 2, -size.cy / 2);
	m_image.Draw(graphics, center.x, center.y);
}