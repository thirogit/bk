#include "stdafx.h"
#include "PngMenuButton.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"
#include "../../utils/TempIconLoader.h"

const int nDropBtnWidth = 16;


CPngMenuBtn::CPngMenuBtn()
{
	m_bPushed = FALSE;
	m_bMenuPushed = FALSE;
	m_IsMenuEnabled = TRUE;
	m_menu = NULL;

}

CPngMenuBtn::~CPngMenuBtn()
{
	if (m_menu != NULL)
		delete m_menu;
}


BEGIN_MESSAGE_MAP(CPngMenuBtn, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

BOOL CPngMenuBtn::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= BS_OWNERDRAW; // Enforce
	m_pParentWnd = pParentWnd;
	if (CButton::Create(L"", dwStyle, rect, pParentWnd, nID) == 0)
	{
		return 0;
	}

	return -1;
}

void CPngMenuBtn::OnStyleChanged(int styleType, LPSTYLESTRUCT style)
{
	if (GWL_EXSTYLE == styleType)
	{
		/*if ((style->styleNew & HBS_EX_NOEDITHENT) ^ (style->styleOld & HBS_EX_NOEDITHENT))
		{
			if (m_menu.m_hMenu)
			{
				m_menu.DestroyMenu();
			}
			CreateMenu();
		}*/
	}
}



void CPngMenuBtn::SetImage(UINT nResPNG)
{
	m_btnImage.LoadPNG(nResPNG);
}


void CPngMenuBtn::SetMenu(const MenuBuilder& menu)
{
	if (m_menu != NULL)
		delete m_menu;
	m_menu = new MenuBuilder(menu);
}


void CPngMenuBtn::DrawImage(Gdiplus::Graphics& graphics, const CRect& drawRect)
{
	CPoint center = drawRect.CenterPoint();
	CSize size = m_btnImage.GetSize();
	center.Offset(-size.cx / 2, -size.cy / 2);
	m_btnImage.Draw(graphics, center.x, center.y);

}


void CPngMenuBtn::DrawItem(DRAWITEMSTRUCT* lpDIS)
{
	if (lpDIS->CtlType != ODT_BUTTON)
		return;

	CFont *pFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	CDC dcMem;
	CBitmap bmp;

	CRect btnRect(lpDIS->rcItem);

	CRect trueRect(btnRect);

	CDC *pDC = CDC::FromHandle(lpDIS->hDC);

	////////////////////////////////////////
	// Button Background                  //
	////////////////////////////////////////
	pDC->FillRect(trueRect, &CBrush(GetSysColor(COLOR_BTNFACE)));

	BOOL bDisabled = ODS_DISABLED & lpDIS->itemState;

	CRect rectContent(btnRect);

	//rectContent.DeflateRect(2,1);

	//if (!m_bMenuPushed)
	//	rectFocus.OffsetRect(m_bPushed,m_bPushed);

	rectContent.right -= nDropBtnWidth;
	rectContent.left += 1;

	////////////////////////////////////////
	// Button in a normal state           //
	////////////////////////////////////////
	if (!m_bPushed || m_bMenuPushed)
		pDC->DrawFrameControl(&btnRect, DFC_BUTTON, DFCS_BUTTONPUSH);


	////////////////////////////////////////
	// Default Button State               //
	////////////////////////////////////////
	if (m_bPushed && !bDisabled)
	{
		pDC->DrawFrameControl(&lpDIS->rcItem,
			DFC_BUTTON,
			DFCS_BUTTONPUSH | DFCS_ADJUSTRECT | DFCS_PUSHED);


		//pDC->FrameRect(&lpDIS->rcItem, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));

		//if (m_bPushed && !m_bMenuPushed)
		//	pDC->FrameRect(&btnRect, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
	}


	////////////////////////////////////////
	// State Focus                        //
	////////////////////////////////////////
	//if (lpDIS->itemState & ODS_FOCUS || m_bPushed) 
	//	if (!m_bMenuPushed)
	//		pDC->DrawFocusRect(&rectFocus);


	////////////////////////////////////////
	// Action Focus                       //
	////////////////////////////////////////
	//if ((lpDIS->itemAction & ODA_FOCUS))
	//	if (!m_bMenuPushed)
	//		pDC->DrawFocusRect(&rectFocus);


	////////////////////////////////////////
	// Draw out bitmap                    //
	////////////////////////////////////////

	// Draw out bitmap


	CSize imgSize = m_btnImage.GetSize();

	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	int imgY = (rectContent.Height() - imgSize.cy) / 2;
	const int marginX = 3;
	if (!bDisabled)
	{
		int imgX = rectContent.left + ((rectContent.Width() - imgSize.cx)/2);
		CPoint lt(imgX + m_bPushed, imgY + m_bPushed);
		DrawImage(graphics,CRect(lt,lt + imgSize));
	}
	else
	{
		COLORREF bkgClr = ::GetSysColor(COLOR_3DFACE);
		HBITMAP img = m_btnImage.GetBitmap24(bkgClr);
		pDC->DrawState(CPoint(marginX + rectContent.left + m_bPushed, imgY + m_bPushed), imgSize, img, DST_BITMAP | DSS_DISABLED);
		DeleteObject(img);
	}

	CRect rectSplit(btnRect);
	rectSplit.DeflateRect(2, 2);
	rectSplit.right -= nDropBtnWidth;


	////////////////////////////////////////
	// Drop down split                    //
	////////////////////////////////////////
	CPen brFace(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	pDC->SelectObject(&brFace);
	pDC->MoveTo(rectSplit.right, rectSplit.top);
	pDC->LineTo(rectSplit.right, rectSplit.bottom);


	CPen brLite(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	pDC->SelectObject(&brLite);
	pDC->MoveTo(rectSplit.right + 1, rectSplit.top);
	pDC->LineTo(rectSplit.right + 1, rectSplit.bottom);


	rectSplit.left = rectSplit.right;
	rectSplit.right += nDropBtnWidth;

	CPoint pt(rectSplit.CenterPoint());
	pt += CPoint(m_bPushed, m_bPushed);

	CPen penBlack(PS_SOLID, 1, (bDisabled || !m_IsMenuEnabled) ? GetSysColor(COLOR_GRAYTEXT) : GetSysColor(COLOR_WINDOWTEXT));
	pDC->SelectObject(&penBlack);
	DrawArrow(pDC, pt);

	////////////////////////////////////////
	// Drop down state                    //
	////////////////////////////////////////
	if (m_bMenuPushed && !bDisabled)
	{
		rectSplit.InflateRect(1, 1);
		pDC->DrawEdge(rectSplit, BDR_SUNKENOUTER, BF_RECT);
	}
}

void CPngMenuBtn::DrawArrow(CDC* pDC, CPoint ArrowTip)
{

	CPoint ptDest;

	CPen* pPen = pDC->GetCurrentPen();
	LOGPEN logPen;

	//	CPen newPen(PS_SOLID,1,RGB(
	pPen->GetLogPen(&logPen);
	pDC->SetPixel(ArrowTip, logPen.lopnColor);


	ArrowTip -= CPoint(1, 1);
	pDC->MoveTo(ArrowTip);

	ptDest = ArrowTip;
	ptDest += CPoint(3, 0);
	pDC->LineTo(ptDest);

	ArrowTip -= CPoint(1, 1);
	pDC->MoveTo(ArrowTip);

	ptDest = ArrowTip;
	ptDest += CPoint(5, 0);
	pDC->LineTo(ptDest);

	ArrowTip -= CPoint(1, 1);
	pDC->MoveTo(ArrowTip);

	ptDest = ArrowTip;
	ptDest += CPoint(7, 0);
	pDC->LineTo(ptDest);
}


void CPngMenuBtn::OnLButtonDown(UINT nFlags, CPoint point)
{

	if (m_bMenuPushed)
	{
		m_bMenuPushed = FALSE;
		Invalidate();
		return;
	}

	if (HitMenuBtn(point) && m_IsMenuEnabled)
	{
		m_bMenuPushed = TRUE;
		SetFocus();
		Invalidate();

		CRect rc;
		GetWindowRect(rc);

		int x = rc.right - nDropBtnWidth;
		int y = rc.bottom;

		if (m_menu)
		{
			m_menu->trackAt(x,y,this);
		}

		m_bMenuPushed = FALSE;
	}
	else
	{
		m_bPushed = TRUE;
	}

	Invalidate();

	if (m_bPushed)
		CButton::OnLButtonDown(nFlags, point);
}

void CPngMenuBtn::OnLButtonUp(UINT nFlags, CPoint point)
{

	if (m_bPushed)
	{
		m_bPushed = FALSE;
		ReleaseCapture();
		Invalidate();
		m_pParentWnd->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CPngMenuBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bPushed)
	{
		ClientToScreen(&point);

		if (WindowFromPoint(point) != this)
		{
			m_bPushed = FALSE;
			ReleaseCapture();
			Invalidate();
		}
	}

	CButton::OnMouseMove(nFlags, point);
}

void CPngMenuBtn::OnSetFocus(CWnd* pOldWnd)
{
	CButton::OnSetFocus(pOldWnd);
	Invalidate();
}

void CPngMenuBtn::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
}

void CPngMenuBtn::OnSysColorChange()
{
	CButton::OnSysColorChange();
	Invalidate();
}

BOOL CPngMenuBtn::HitMenuBtn(CPoint point)
{
	ClientToScreen(&point);

	CRect rect;
	GetWindowRect(rect);
	rect.left = rect.right - nDropBtnWidth;

	return rect.PtInRect(point);
}


void CPngMenuBtn::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW); // Enforce
	m_pParentWnd = GetParent();
	CButton::PreSubclassWindow();
}

