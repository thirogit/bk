#include "stdafx.h"
#include "ArrowButton.h"


BEGIN_MESSAGE_MAP(CArrowButton, CButton)

	ON_WM_ERASEBKGND()
	

END_MESSAGE_MAP()

CArrowButton::CArrowButton() : m_arrowStyle(ABS_ARROW_UP)
{

}

CArrowButton::~CArrowButton()
{

}

void CArrowButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
	CButton::PreSubclassWindow();
}

//CSize CArrowButton::GetArrowSize()
//{
//	// Get size dimensions of edges
//	CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));
//
//	// Get size of dropdown arrow
//	int nArrowWidth  = ::GetSystemMetrics(SM_CXHTHUMB) / 3 + 2;
//	int nArrowHeight = ::GetSystemMetrics(SM_CYVTHUMB) / 3 + 2;
//	CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));
//
//	// Make sure arrow size is an odd number (just looks neater)
//	if ((ArrowSize.cx & 1) == 0)
//	{
//		ArrowSize.cx++;
//		ArrowSize.cy++;
//	}
//
//	return ArrowSize;
//}


void CArrowButton::SetArrowStyle(WORD style)
{
	m_arrowStyle = style;
	Invalidate(TRUE);
}

WORD CArrowButton::GetArrowStyle()
{
	return m_arrowStyle;
}

BOOL CArrowButton::OnEraseBkgnd(CDC* pDC)
{
    CRect rectClient;        // Rectangle for entire client area
    GetClientRect(&rectClient);

    // Fill the rectangle
    //pDC->FillSolidRect(&rectClient, m_crColor);

	return TRUE;
}


//const int g_ciArrowSizeX = 4 ;
//const int g_ciArrowSizeY = 2 ;
//const int g_ciArrowSizeX = 7;
//const int g_ciArrowSizeY = 4;

void CArrowButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct);

	CDC*    pDC      = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT    state    = lpDrawItemStruct->itemState;
    CRect   rDraw    = lpDrawItemStruct->rcItem;
	CRect	rArrow;

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

	//******************************************************
	//**                     Draw Arrow
	//******************************************************
	
	int nArrowWidth = 7;
	int nArrowHeight = (nArrowWidth / 2 + nArrowWidth % 1);

	int midX = ((rDraw.left + rDraw.right) / 2) - 1;
	int midY = (rDraw.bottom + rDraw.top) / 2;


	if (m_arrowStyle & ABS_ARROW_UP || m_arrowStyle & ABS_ARROW_DOWN)
	{

		
		rArrow.left = midX - nArrowWidth / 2;
		rArrow.right = midX + nArrowWidth / 2;

		rArrow.top = midY - nArrowHeight / 2;
		rArrow.bottom = midY + nArrowHeight / 2;


		int direction = (m_arrowStyle & ABS_ARROW_UP) ? 1 : -1;

		int nArrowLeft = rArrow.left;
		int nArrowTop = rArrow.top + direction*(nArrowHeight / 2);
		while (nArrowWidth > 0)
		{
			CPoint point(nArrowLeft, nArrowTop);
			nArrowLeft += 1;
			nArrowTop -= direction;
			pDC->MoveTo(point);
			point.x += nArrowWidth;
			nArrowWidth -= 2;
			pDC->LineTo(point);
		}

	}
	else
	{
		rArrow.left = midX - nArrowHeight / 2;
		rArrow.right = midX + nArrowHeight / 2;

		rArrow.top = midY - nArrowWidth / 2;
		rArrow.bottom = midY + nArrowWidth / 2;

		int direction = (m_arrowStyle & ABS_ARROW_LEFT) ? 1 : -1;

		int nArrowTop = rArrow.top;
		int nArrowLeft = rArrow.left + direction*(nArrowHeight / 2);
		   
		while (nArrowWidth > 0)
		{
			CPoint point(nArrowLeft, nArrowTop);
			nArrowLeft += 1;
			nArrowTop -= direction;
			pDC->MoveTo(point);
			point.x += nArrowWidth;
			nArrowWidth -= 2;
			pDC->LineTo(point);
		}

	}
	
}
