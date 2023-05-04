#include "stdafx.h"
#include "PrintPreviewCtrl.h"
#include <set>
#include "../runtime/WinPaperUtils.h"

BEGIN_MESSAGE_MAP(CPreviewViewCtrl, CWnd)

	ON_WM_SIZE()        
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()

END_MESSAGE_MAP()

#define PRINTPREVIEW_CLASSNAME  _T( "PRINTPREVIEW" )

CPreviewViewCtrl::CPreviewViewCtrl()
{
	
	RegisterWindowClass();
	
	m_pPrinterDlg = NULL;
	m_pPreviewDC = NULL;

	m_hMagnifyCursor = NULL;
	
	m_nZoomState = _AFX_ZOOM_OUT;

	m_pPrintOut = NULL;

	m_xScroll = 0;
	m_yScroll = 0;
}

bool CPreviewViewCtrl::RegisterWindowClass()
{
	WNDCLASS wndcls;

    HINSTANCE hInst = AfxGetResourceHandle();

    if ( !( ::GetClassInfo( hInst, PRINTPREVIEW_CLASSNAME , &wndcls ) ) )
    {		 
       wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
       wndcls.lpfnWndProc      = ::DefWindowProc;
       wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
       wndcls.hInstance        = hInst;
       wndcls.hIcon            = NULL;
       wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
       wndcls.hbrBackground    = (HBRUSH)( COLOR_3DFACE + 1 );
       wndcls.lpszMenuName     = NULL;
       wndcls.lpszClassName    = PRINTPREVIEW_CLASSNAME;
        
       if( !AfxRegisterClass( &wndcls ) )
       {
          AfxThrowResourceException();
          return FALSE;
       }
    }
    return TRUE;
}

void CPreviewViewCtrl::UnregisterWindowClass()
{
	WNDCLASS wndcls = { 0 };
   if( ( ::GetClassInfo( AfxGetResourceHandle() , PRINTPREVIEW_CLASSNAME , &wndcls ) ) ) 
   {
      ::UnregisterClass( wndcls.lpszClassName , AfxGetResourceHandle() );
   }
}

BOOL CPreviewViewCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID ) 
{
	return CWnd::Create(PRINTPREVIEW_CLASSNAME,L"",dwStyle,rect,pParentWnd,nID);
}

CPreviewViewCtrl::~CPreviewViewCtrl()
{
	m_dcPrint.Detach();         // print DC is deleted by CPrintInfo destructor

	DisposePrintDlg();
	
	delete m_pPreviewDC;        // Get rid of preview DC object

	if (m_hMagnifyCursor != NULL)
	{
		// make sure that m_hMagnifyCursor isn't the current cursor when we destroy it
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		DestroyCursor(m_hMagnifyCursor);
	}
	UnregisterWindowClass();
}

int CPreviewViewCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int retVal = CWnd::OnCreate(lpCreateStruct);
	if (retVal == -1)
		return -1;      // if -1 bag out	
		
	return retVal;
}

void CPreviewViewCtrl::DisposePrintDlg()
{
	if (m_pPrinterDlg != NULL && m_pPrinterDlg->m_pd.hDC != NULL)
	{
		::DeleteDC(m_pPrinterDlg->m_pd.hDC);
		m_pPrinterDlg->m_pd.hDC = NULL;
	}
	delete m_pPrinterDlg;
	m_pPrinterDlg = NULL;
}

bool CPreviewViewCtrl::InitializeDefaultPrinter(CPrintDialog* pPrinterDlg)
{
	CWinApp* pApp = AfxGetApp();
	if (pPrinterDlg->m_pd.hDC == NULL)
	{
		// if no printer set then, get default printer DC and create DC without calling
		//   print dialog.
		if (!pApp->GetPrinterDeviceDefaults(&pPrinterDlg->m_pd))
		{
			// bring up dialog to alert the user they need to install a printer.
			if (pApp->DoPrintDialog(pPrinterDlg) != IDOK)
				return FALSE;
		}
		
	}	
	return TRUE;
}

void CPreviewViewCtrl::SetPaper(CPrintDialog* pPrinterDlg,const bkprint::Paper& paper)
{
	LPDEVMODE pDM;
	if ( (pPrinterDlg->m_pd.hDevMode) &&  (pDM = (DEVMODE*)GlobalLock(pPrinterDlg->m_pd.hDevMode)) )
	{     
		pDM->dmOrientation = WinPaperUtils::GetOrientation(paper.GetOrientation()); 
		pDM->dmPaperSize = WinPaperUtils::GetPaperSize(paper.GetType());
		GlobalUnlock(pPrinterDlg->m_pd.hDevMode);
	}
}

BOOL CPreviewViewCtrl::CreateCanvas(CPrintDialog* pPrinterDlg)
{
	if (pPrinterDlg->m_pd.hDC == NULL)
	{
		// call CreatePrinterDC if DC was not created by above
		if (pPrinterDlg->CreatePrinterDC() == NULL)
			return FALSE;
	}
	return TRUE;
}

bool CPreviewViewCtrl::SetPrintOut(bkprint::PrintOut* pPrintOut)
{
	m_dcPrint.Detach();
	DisposePrintDlg();
	delete m_pPreviewDC; 

	m_pPreviewDC = new CPreviewDC;      
	m_pPrinterDlg  = new CPrintDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOSELECTION);

	m_pPrinterDlg->m_pd.nMinPage = 1;
	m_pPrinterDlg->m_pd.nMaxPage = -1;

	if (!InitializeDefaultPrinter(m_pPrinterDlg))
		return FALSE;

	SetPaper(m_pPrinterDlg,pPrintOut->GetPaper());
		

	if(!CreateCanvas(m_pPrinterDlg))
		return FALSE;
	
	m_dcPrint.Attach(m_pPrinterDlg->m_pd.hDC);
	m_pPreviewDC->SetAttribDC(m_pPrinterDlg->m_pd.hDC);

	m_pPreviewDC->m_bPrinting = TRUE;
	m_dcPrint.m_bPrinting = TRUE;

	//m_dcPrint.SaveDC();     // Save pristine state of DC

	//HDC hDC = ::GetDC(m_hWnd);
	//m_pPreviewDC->SetOutputDC(hDC);

	//m_pPrintView->OnBeginPrinting(m_pPreviewDC, m_pPreviewInfo);

	//m_pPreviewDC->ReleaseOutputDC();
	//::ReleaseDC(m_hWnd, hDC);

	//m_dcPrint.RestoreDC(-1);    // restore to untouched state

	// Get Pixels per inch from Printer
	m_sizePrinterPPI.cx = m_dcPrint.GetDeviceCaps(LOGPIXELSX);
	m_sizePrinterPPI.cy = m_dcPrint.GetDeviceCaps(LOGPIXELSY);

	//m_nPages = m_pPreviewInfo->m_nNumPreviewPages;
	//if (m_nPages == 0)
	//	m_nPages = 1;
	//else if (m_nPages > m_nMaxPages)
	//	m_nPages = m_nMaxPages;     // Sanity Check!

	//m_nZoomOutPages = m_nPages;

	//SetScrollSizes(MM_TEXT, CSize(1, 1));   // initialize mapping mode only

	//if (m_pPreviewInfo->GetMaxPage() < 0x8000 &&
	//	m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() <= 32767U)
	//{
	//	SCROLLINFO info;
	//	info.fMask = SIF_PAGE|SIF_RANGE;
	//	info.nMin = m_pPreviewInfo->GetMinPage();
	//	info.nMax = m_pPreviewInfo->GetMaxPage();
	//	info.nPage = 1;
	//	if (!SetScrollInfo(SB_VERT, &info, FALSE))
	//		SetScrollRange(SB_VERT, info.nMin, info.nMax, FALSE);
	//}
	//else
	//	ShowScrollBar(SB_VERT, FALSE);      // if no range specified, or too
	//										// large don't show

	m_pPrintOut = pPrintOut;
	SetCurrentPage(0, TRUE);
	return TRUE;
}

void CPreviewViewCtrl::OnSize(UINT nType, int cx, int cy)
{
	// CScrollView handles everything if zoomed in.
	if (m_nZoomState == _AFX_ZOOM_OUT)
	{
		// Force recalc of scale ratios on next draw
		m_PageInfo.sizeScaleRatio.cx = 0;  // zero scale ratios
		CWnd::OnSize(nType, cx, cy);       // No scroll functionality
	}
	else
	{
		// adjust scroll size to size of page
		//m_pageDev.cx = cx;
		//m_pageDev.cy = cy;
		//m_lineDev.cx = cx / 10;
		//m_lineDev.cy = cy / 10;
		CWnd::OnSize(nType, cx, cy);
	}
}


#define PREVIEW_MARGIN  8
#define PREVIEW_PAGEGAP 8

// Return is actually the fraction cx/cy. Simply using CSize for convenience
CSize CPreviewViewCtrl::CalcScaleRatio(CSize screenSize, CSize actualSize)
{
	// Test ratio based on vertical dimension to see if it is the one to use
	int nNum = screenSize.cy;
	int nDen = actualSize.cy;

	// If scaled width too large, choose width as primary dimension
	if (MulDiv(actualSize.cx, nNum, nDen) > screenSize.cx)
	{
		// wrong ratio--base on width
		nNum = screenSize.cx;
		nDen = actualSize.cx;
	}
	CSize ratio(nNum, nDen);
	return ratio;
}

// Position Page...
// Generate a Screen MM_TEXT rectangle to enclose each page.  Dimensions
// of the rectangle must be 1 pixel Above and Left of the top/left corner
// of the page and the rectangle width and height must be THREE pixels
// larger than page in order to provide the correct placement of the
// two pixel border.
//
// This routine is called once for each page with the preview DC set up for
// that page

void CPreviewViewCtrl::PositionPage(UINT nPage)
{
	CSize windowSize = CalcPageDisplaySize();

	m_dcPrint.Escape(GETPHYSPAGESIZE, 0, NULL, (LPVOID)&m_PageInfo.sizeUnscaled);

	CSize* pSize = &m_PageInfo.sizeUnscaled;


	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);

	// Convert page size to screen coordinates
	pSize->cx = MulDiv(pSize->cx, cxPixelsPerInch, m_sizePrinterPPI.cx);
	pSize->cy = MulDiv(pSize->cy, cyPixelsPerInch, m_sizePrinterPPI.cy);

	m_PageInfo.sizeZoomOutRatio = CalcScaleRatio(windowSize, *pSize);

	SetScaledSize(nPage);
}

CSize CPreviewViewCtrl::CalcPageDisplaySize()
	// calculate the current page size
	//  set 'm_nSecondPageOffset' to start of second page
	// return size of current page less margins
{
	CSize windowSize, scrollSize;
	GetTrueClientSize(windowSize, scrollSize);

	// subtract out vertical scrollbar if zoomed out and page range is known
	// and there is more than one page.
	//if (m_nZoomState == _AFX_ZOOM_OUT && (m_pPreviewInfo->GetMaxPage() != 0xffff) &&
	//	(m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() != 0))
	//	windowSize.cx -= scrollSize.cx;

	windowSize.cx = windowSize.cx - 2*PREVIEW_MARGIN;

	windowSize.cy -= 2*PREVIEW_MARGIN;
	return windowSize;
}

void CPreviewViewCtrl::SetScaledSize(UINT nPage)
{
	CSize* pSize = &m_PageInfo.sizeUnscaled;
	CSize* pRatio = &m_PageInfo.sizeScaleRatio;
	CSize* pZoomOutRatio = &m_PageInfo.sizeZoomOutRatio;
	CSize windowSize = CalcPageDisplaySize();
	BOOL bPaperLarger = pZoomOutRatio->cx < pZoomOutRatio->cy;
		// whether the paper is larger than the screen, or vice versa

	switch (m_nZoomState)
	{
	case _AFX_ZOOM_OUT:
		*pRatio = *pZoomOutRatio;
		break;

	case _AFX_ZOOM_MIDDLE:
		// the middle zoom state is a ratio between cx/cy and
		// 1/1 (or cy/cy).  It is, therefore:
		//
		// (cx + cy)/2
		// -----------
		//     cy
		//
		// if the paper is larger than the screen, or
		//
		// (3*cx - cy)/2
		// -------------
		//      cy
		//
		// if the paper is smaller than the screen.
		if (bPaperLarger)
		{
			pRatio->cy = pZoomOutRatio->cy;
			pRatio->cx = (pZoomOutRatio->cx + pRatio->cy) / 2;
		}
		else
		{
			pRatio->cy = pZoomOutRatio->cy;
			pRatio->cx = (3*pZoomOutRatio->cx - pRatio->cy) / 2;
		}
		break;

	case _AFX_ZOOM_IN:
		if (bPaperLarger)
			pRatio->cx = pRatio->cy = 1;
		else
		{
			// if the paper is smaller than the screen space we're displaying
			// it in, then using a ratio of 1/1 will result in a smaller image
			// on the screen, not a larger one. To get a larger image in this
			// case we double the zoom out ratio.
			pRatio->cy = pZoomOutRatio->cy;
			pRatio->cx = 2*pZoomOutRatio->cx - pZoomOutRatio->cy;
		}
		break;

	default:
		ASSERT(FALSE);
	}

	// Convert to scaled size
	CSize scaledSize;
	scaledSize.cx = MulDiv(pSize->cx, pRatio->cx, pRatio->cy);
	scaledSize.cy = MulDiv(pSize->cy, pRatio->cx, pRatio->cy);

	CRect* pRect = &m_PageInfo.rectScreen;
	pRect->SetRect(PREVIEW_MARGIN, PREVIEW_MARGIN,
				   scaledSize.cx + PREVIEW_MARGIN + 3,
				   scaledSize.cy + PREVIEW_MARGIN + 3);

	if (m_nZoomState == _AFX_ZOOM_OUT)
	{
		pRect->OffsetRect((windowSize.cx - pRect->Size().cx) / 2 - 1,
						  (windowSize.cy - pRect->Size().cy) / 2 - 1);

		//if (nPage == 1)
		//	pRect->OffsetRect(m_nSecondPageOffset, 0);
	}
	else
	{
		SetScrollSizes(pRect->Size() + CSize(PREVIEW_MARGIN * 2, PREVIEW_MARGIN * 2), windowSize);
	}
}

void CPreviewViewCtrl::SetScrollSizes(const CSize& page,const CSize& window)
{
	int xRange = page.cx - window.cx ;
	int yRange = page.cy - window.cy ;

	if(xRange > 0)
	{
		SCROLLINFO horz;
		ShowScrollBar(SB_HORZ, TRUE);

		horz.cbSize = sizeof(SCROLLINFO);
		horz.fMask = SIF_ALL;
		horz.nMin = 0;
		horz.nMax = xRange;
		horz.nPage = 0;
		horz.nPos = 0;
		horz.nTrackPos=0;

		SetScrollInfo(SB_HORZ,&horz);
	}

	if(yRange > 0)
	{
		SCROLLINFO vert;
		ShowScrollBar(SB_VERT, TRUE); 

		vert.cbSize = sizeof(SCROLLINFO);
		vert.fMask = SIF_ALL;
		vert.nMin = 0;
		vert.nMax = yRange;
		vert.nPage = 0;
		vert.nPos = 0;
		vert.nTrackPos=0;

		SetScrollInfo(SB_VERT,&vert);		
	}

}

void CPreviewViewCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(SB_THUMBTRACK == nSBCode)
	{
		int nCurrPos = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT,nPos);
		//m_xScroll = nPos;		
		ScrollWindow(0,-(nPos-nCurrPos));
		UpdateWindow();
		
	}
	//InvalidateRect(&rectStaticClient);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPreviewViewCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{	
	if(SB_THUMBTRACK == nSBCode)
	{
		int nCurrPos = GetScrollPos(SB_HORZ);
		SetScrollPos(SB_HORZ,nPos);		
		
		ScrollWindow(-(nPos-nCurrPos),0);
		UpdateWindow();
		//Invalidate(TRUE);
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPreviewViewCtrl::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	int cxVScroll = GetSystemMetrics(SM_CXVSCROLL) + AFX_CX_BORDER;
	int cyHScroll = GetSystemMetrics(SM_CYHSCROLL) + AFX_CY_BORDER;


	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = cxVScroll;
		if (dwStyle & WS_BORDER)
			sizeSb.cx -= AFX_CX_BORDER;
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = cyHScroll;
		if (dwStyle & WS_BORDER)
			sizeSb.cy -= AFX_CY_BORDER;
	}
}

BOOL CPreviewViewCtrl::GetTrueClientSize(CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

BOOL CPreviewViewCtrl::OnEraseBkgnd(CDC* pDC)
{
	ASSERT_VALID(pDC);

	// Fill background with APPWORKSPACE
	CBrush backBrush(GetSysColor(COLOR_APPWORKSPACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

//http://msdn.microsoft.com/en-us/library/windows/desktop/hh298420%28v=vs.85%29.aspx

void CPreviewViewCtrl::OnPaint()
{
	CPaintDC dc(this);

	// don't do anything if not fully initialized
	if (m_pPrintOut == NULL || m_dcPrint.m_hDC == NULL)
		return;


	CPoint ptVpOrg(0, 0);       // assume no shift for printing
	ptVpOrg = -GetDeviceScrollPosition();

	//if (m_nZoomState == _AFX_ZOOM_OUT)
	//{
	//	TRACE("ZoomOut scroolPos = (%d,%d)\n",ptVpOrg.x,ptVpOrg.y);
	//}

		//if (m_bCenter)
		//{
		//	CRect rect;
		//	GetClientRect(&rect);

		//	// if client area is larger than total device size,
		//	// override scroll positions to place origin such that
		//	// output is centered in the window
		//	if (m_totalDev.cx < rect.Width())
		//		ptVpOrg.x = (rect.Width() - m_totalDev.cx) / 2;
		//	if (m_totalDev.cy < rect.Height())
		//		ptVpOrg.y = (rect.Height() - m_totalDev.cy) / 2;
		//}
	
	dc.SetViewportOrg(ptVpOrg);

	CPoint ViewportOrg = dc.GetViewportOrg();
	//ViewportOrg.x -= m_xScroll;
	
	//TRACE("Viewport = (%d,%d)\n",ViewportOrg.x,ViewportOrg.y);

	CPen rectPen;
	rectPen.CreatePen(PS_SOLID, 2, GetSysColor(COLOR_WINDOWFRAME));
	CPen shadowPen;
	shadowPen.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_BTNSHADOW));

		int nSavedState = m_dcPrint.SaveDC();       // Save pristine state of DC

		// Use paint DC for print preview output
		m_pPreviewDC->SetOutputDC(dc.GetSafeHdc());

		dc.SaveDC();          // save the output dc state

		CSize* pRatio = &m_PageInfo.sizeScaleRatio;
		CRect* pRect = &m_PageInfo.rectScreen;

		if (pRatio->cx == 0)
		{   // page position has not been determined
			PositionPage(0);    // compute page position
			if (m_nZoomState != _AFX_ZOOM_OUT)
			{
				ViewportOrg = -GetDeviceScrollPosition();
				//if (false)//m_bCenter)
				//{
				//	CRect rect;
				//	GetClientRect(&rect);
				//	// if client area is larger than total device size,
				//	// override scroll positions to place origin such that
				//	// output is centered in the window
				//	if (m_totalDev.cx < rect.Width())
				//		ViewportOrg.x = (rect.Width() - m_totalDev.cx) / 2;
				//	if (m_totalDev.cy < rect.Height())
				//		ViewportOrg.y = (rect.Height() - m_totalDev.cy) / 2;
				//}
			}
		}

		dc.SetMapMode(MM_TEXT);   // Page Rectangle is in screen device coords
		dc.SetViewportOrg(ViewportOrg);
		dc.SetWindowOrg(0, 0);

		dc.SelectStockObject(HOLLOW_BRUSH);
		dc.SelectObject(&rectPen);
		dc.Rectangle(pRect);

		dc.SelectObject(&shadowPen);

		dc.MoveTo(pRect->right + 1, pRect->top + 3);
		dc.LineTo(pRect->right + 1, pRect->bottom + 1);
		dc.MoveTo(pRect->left + 3, pRect->bottom + 1);
		dc.LineTo(pRect->right + 1, pRect->bottom + 1);

		// erase background to white (most paper is white)
		CRect rectFill = *pRect;
		rectFill.left += 1;
		rectFill.top += 1;
		rectFill.right -= 2;
		rectFill.bottom -= 2;
		::FillRect(dc.m_hDC, rectFill, (HBRUSH)GetStockObject(WHITE_BRUSH));

		dc.RestoreDC(-1);     // restore to synchronized state

		// Set scale ratio for this page
		m_pPreviewDC->SetScaleRatio(pRatio->cx, pRatio->cy);

		CSize PrintOffset;
		VERIFY(m_pPreviewDC->Escape(GETPRINTINGOFFSET, 0, NULL, (LPVOID)&PrintOffset));
		m_pPreviewDC->PrinterDPtoScreenDP((LPPOINT)&PrintOffset);
		PrintOffset += (CSize)pRect->TopLeft();
		PrintOffset += CSize(1, 1);
		PrintOffset += (CSize)ViewportOrg;  // For Scrolling

		m_pPreviewDC->SetTopLeftOffset(PrintOffset);
		
		m_pPreviewDC->ClipToPage();

		m_pPreviewDC->SetMapMode(MM_ANISOTROPIC);
		m_pPreviewDC->SetViewportExt(m_pPreviewDC->GetDeviceCaps(LOGPIXELSX),m_pPreviewDC->GetDeviceCaps(LOGPIXELSY));
		m_pPreviewDC->SetWindowExt(254, 254);

		/*CRect rectDraw(0, 0,m_pPreviewDC->GetDeviceCaps(HORZRES),m_pPreviewDC->GetDeviceCaps(VERTRES));
		m_pPreviewDC->DPtoLP(&rectDraw);

		m_pPreviewDC->SetWindowOrg(	-rectDraw.Width()/2, -rectDraw.Height()/2 );*/
		
		m_pPrintOut->DrawPage(m_pPreviewDC, m_nCurrentPage);

		//m_pPrintView->OnPrint(m_pPreviewDC, m_pPreviewInfo);

		m_pPreviewDC->ReleaseOutputDC();

		m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state



	rectPen.DeleteObject();
	shadowPen.DeleteObject();
}


void CPreviewViewCtrl::OnNextPage()
{
	SetCurrentPage(m_nCurrentPage + 1, TRUE);
}

void CPreviewViewCtrl::OnPrevPage()
{
	SetCurrentPage(m_nCurrentPage - 1, TRUE);
}


CPoint CPreviewViewCtrl::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);
	return pt;
}

// Finds page pointed to and convert to 1:1 screen device units
BOOL CPreviewViewCtrl::InPageRect(const CPoint& point)
{
	CPoint absPoint = point;
		
	CRect rectClient;
	this->GetClientRect(&rectClient);
	CRect rectScreen(m_PageInfo.rectScreen);
	/* When zoom state is not _AFX_ZOOM_OUT, only one page can be displayed. 
		Two-page is available only on state _AFX_ZOOM_OUT 
		   
		When zoom state is not _AFX_ZOOM_OUT, the rectScreen need to be adjusted for point testing */
	if (m_nZoomState != _AFX_ZOOM_OUT)
	{
		absPoint += (CSize)GetDeviceScrollPosition();
		int nDeltaX = (rectClient.Size().cx - rectScreen.Size().cx - 2 * PREVIEW_MARGIN) / 2 - 1;
		int nDeltaY = (rectClient.Size().cy - rectScreen.Size().cy - 2 * PREVIEW_MARGIN) / 2 - 1;

		// When delta is less than 0, don't need to change the value.
		nDeltaX = nDeltaX < 0 ? 0 : nDeltaX;
		nDeltaY = nDeltaY < 0 ? 0 : nDeltaY;
		rectScreen.OffsetRect(nDeltaX, nDeltaY);
	}

	return rectScreen.PtInRect(absPoint);
}


void CPreviewViewCtrl::OnLButtonDown(UINT, CPoint point)
{
	if (!InPageRect(point))
		return;                         // Didn't click on a page

	// Set new zoom state
	SetZoomState((m_nZoomState == _AFX_ZOOM_IN) ? _AFX_ZOOM_OUT : m_nZoomState + 1,	point);
}

void CPreviewViewCtrl::SetZoomState(UINT nNewState, CPoint point)
{
	if (m_nZoomState != nNewState)
	{
		m_nZoomState = nNewState;
		DoZoom(point);
	}
}

void CPreviewViewCtrl::OnZoomIn()
{
	if (m_nZoomState != _AFX_ZOOM_IN)
		SetZoomState(m_nZoomState + 1, CPoint(0, 0));
}

void CPreviewViewCtrl::OnZoomOut()
{
	if (m_nZoomState != _AFX_ZOOM_OUT)
		SetZoomState(m_nZoomState - 1, CPoint(0, 0));
}

// Actual zoom code.
void CPreviewViewCtrl::DoZoom(CPoint point)
{
	if (m_nZoomState == _AFX_ZOOM_OUT)
	{
		SetScrollPos(SB_HORZ,0);
		ShowScrollBar(SB_HORZ, FALSE);
		SetScrollPos(SB_VERT,0);
		ShowScrollBar(SB_VERT, FALSE);       		
		SetCurrentPage(m_nCurrentPage, TRUE);
	}
	else
	{		

		m_PageInfo.sizeZoomOutRatio = m_PageInfo.sizeZoomOutRatio;
		m_PageInfo.sizeUnscaled = m_PageInfo.sizeUnscaled;

		// Sets the printer page
		SetCurrentPage(m_nCurrentPage, FALSE);

		SetScaledSize(0);

		CSize* pRatio = &m_PageInfo.sizeScaleRatio;

		// convert Hit Point from screen 1:1
		point.x = MulDiv(point.x, pRatio->cx, pRatio->cy);
		point.y = MulDiv(point.y, pRatio->cx, pRatio->cy);

		// Adjust point for page position
		point += (CSize)m_PageInfo.rectScreen.TopLeft();

		// Scroll to center
		CenterOnPoint(point);
	}
}

void CPreviewViewCtrl::CenterOnPoint(CPoint ptCenter) // center in device coords
{
	CRect rect;
	GetClientRect(&rect);           // find size of client window

	int xDesired = ptCenter.x - rect.Width() / 2;
	int yDesired = ptCenter.y - rect.Height() / 2;

	DWORD dwStyle = GetStyle();

	if ((dwStyle & WS_HSCROLL) == 0 || xDesired < 0)
	{
		xDesired = 0;
	}
	else
	{
		int xMax = GetScrollLimit(SB_HORZ);
		if (xDesired > xMax)
			xDesired = xMax;
	}

	if ((dwStyle & WS_VSCROLL) == 0 || yDesired < 0)
	{
		yDesired = 0;
	}
	else
	{
		int yMax = GetScrollLimit(SB_VERT);
		if (yDesired > yMax)
			yDesired = yMax;
	}
	
	SetScrollPos(SB_HORZ, xDesired);
	SetScrollPos(SB_VERT, yDesired);
}


UINT CPreviewViewCtrl::GetCurrentPage() const
{
	return m_nCurrentPage;
}

void CPreviewViewCtrl::SetCurrentPage(UINT nPage, BOOL bClearRatios)
{
	m_nCurrentPage = nPage;
	
	if (m_nCurrentPage > m_pPrintOut->GetNumberOfPages())
		m_nCurrentPage = m_pPrintOut->GetNumberOfPages()-1;

	if (m_nCurrentPage < 0)
		m_nCurrentPage = 0;
	
	if (m_nZoomState == _AFX_ZOOM_OUT)
		SetScrollPos(SB_VERT, m_nCurrentPage);

	if (bClearRatios)
	{
		// Force Recalc of layout
		m_PageInfo.sizeScaleRatio.cx = 0;           // zero scale ratios
	}

	Invalidate(TRUE);
}


BOOL CPreviewViewCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest != HTCLIENT)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);     // client coordinates of mouse position

	
	if (m_nZoomState != _AFX_ZOOM_IN && InPageRect(point))
	{                       // On a page and not zoomed all the way in
		if (m_hMagnifyCursor == NULL)
		{
			HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(AFX_IDC_MAGNIFY), ATL_RT_GROUP_CURSOR);
			m_hMagnifyCursor = ::LoadCursorW(hInst,	ATL_MAKEINTRESOURCEW(AFX_IDC_MAGNIFY));
		}
		::SetCursor(m_hMagnifyCursor);
	}
	else
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
	return 0;
}


