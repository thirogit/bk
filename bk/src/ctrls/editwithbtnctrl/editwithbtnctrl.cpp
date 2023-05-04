#include "stdafx.h"
#include "afxcontrolbarutil.h"
#include "afxglobals.h"
#include "afxtoolbarimages.h"
#include "afxribbonres.h"
#include "afxshellmanager.h"
#include "editwithbtnctrl.h"
#include "afxtagmanager.h"
#include "afxctrlcontainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CEditWithBtnCtrl, COXMaskedEdit)

CEditWithBtnCtrl::CEditWithBtnCtrl()
{
	m_rectBtn.SetRectEmpty();
	m_bIsButtonPressed = FALSE;
	m_bIsButtonHighlighted = FALSE;
	m_bIsButtonCaptured = FALSE;	
	m_sizeImage = CSize(0, 0);
	m_nBrowseButtonWidth = 20;	
}

CEditWithBtnCtrl::~CEditWithBtnCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditWithBtnCtrl, COXMaskedEdit)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_MFC_INITCTRL, &CEditWithBtnCtrl::OnInitControl)
END_MESSAGE_MAP()

void CEditWithBtnCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsButtonCaptured)
	{
		ReleaseCapture();

		m_bIsButtonPressed = FALSE;
		m_bIsButtonCaptured = FALSE;
		m_bIsButtonHighlighted = FALSE;

		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		if (m_rectBtn.PtInRect(point))
		{
			OnBtnClick();
		}

		return;
	}

	COXMaskedEdit::OnLButtonUp(nFlags, point);
}

void CEditWithBtnCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsButtonCaptured)
	{
		BOOL bIsButtonPressed = m_rectBtn.PtInRect(point);
		if (bIsButtonPressed != m_bIsButtonPressed)
		{
			m_bIsButtonPressed = bIsButtonPressed;
			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}

		return;
	}

	if (m_bIsButtonHighlighted)
	{
		if (!m_rectBtn.PtInRect(point))
		{
			m_bIsButtonHighlighted = FALSE;
			ReleaseCapture();

			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}

	COXMaskedEdit::OnMouseMove(nFlags, point);
}

void CEditWithBtnCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	COXMaskedEdit::OnNcCalcSize(bCalcValidRects, lpncsp);

	
	lpncsp->rgrc [0].left += m_nBrowseButtonWidth;
	
}

void CEditWithBtnCtrl::OnNcPaint()
{
	COXMaskedEdit::OnNcPaint();	

	CWindowDC dc(this);

	CRect rectWindow;
	GetWindowRect(rectWindow);

	m_rectBtn = rectWindow;
	m_rectBtn.right = m_rectBtn.left +  m_nBrowseButtonWidth;

	CRect rectClient;
	GetClientRect(rectClient);
	ClientToScreen(&rectClient);

	m_rectBtn.OffsetRect((rectClient.left - m_nBrowseButtonWidth) - rectWindow.left, 0);
	//m_rectBtn.OffsetRect(2, 0);

	m_rectBtn.top += rectClient.top - rectWindow.top;
	m_rectBtn.bottom -= rectWindow.bottom - rectClient.bottom;

	CRect rect = m_rectBtn;
	rect.OffsetRect(-rectWindow.left, -rectWindow.top);

	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect(&rect);

	dc.SelectClipRgn(&rgnClip);

	OnDrawBrowseButton(&dc, rect, m_bIsButtonPressed, m_bIsButtonHighlighted);

	dc.SelectClipRgn(NULL);

	ScreenToClient(&m_rectBtn);
}

LRESULT CEditWithBtnCtrl::OnNcHitTest(CPoint point)
{
	CPoint ptClient = point;
	ScreenToClient(&ptClient);

	if (m_rectBtn.PtInRect(ptClient))
	{
		return HTCAPTION;
	}

	return COXMaskedEdit::OnNcHitTest(point);
}

void CEditWithBtnCtrl::OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bHighlight)
{
	
	ASSERT_VALID(pDC);

	ButtonState state = ButtonsIsRegular;

	if (bIsButtonPressed)
	{
		state =ButtonsIsPressed;
	}
	else if (bHighlight)
	{
		state = ButtonsIsHighlighted;
	}

	COLORREF clrText = GetGlobalData()->clrBtnText;

	if (!OnDrawBrowseButton(pDC, rect, state, clrText))
	{
		return;
	}

	int iImage = 0;

	if (m_ImageBrowse.GetSafeHandle() != NULL)
	{
			
		iImage = 0;
		CPoint ptImage;
		ptImage.x = rect.CenterPoint().x - m_sizeImage.cx / 2;
		ptImage.y = rect.CenterPoint().y - m_sizeImage.cy / 2;

		if (bIsButtonPressed && CMFCVisualManager::GetInstance()->IsOffsetPressedButton())
		{
			ptImage.x++;
			ptImage.y++;
		}

		m_ImageBrowse.Draw(pDC, iImage, ptImage, ILD_NORMAL);
	}
	else
	{
		COLORREF clrTextOld = pDC->SetTextColor(clrText);
		int nTextMode = pDC->SetBkMode(TRANSPARENT);
		CFont* pFont = (CFont*) pDC->SelectStockObject(DEFAULT_GUI_FONT);

		CRect rectText = rect;
		rectText.DeflateRect(1, 2);
		rectText.OffsetRect(0, -2);

		if (bIsButtonPressed)
		{
			rectText.OffsetRect(1, 1);
		}

		pDC->DrawText(_T("..."), rectText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		pDC->SetTextColor(clrTextOld);
		pDC->SetBkMode(nTextMode);
		pDC->SelectObject(pFont);
	}
}

void CEditWithBtnCtrl::EnableButton()
{
	ASSERT_VALID(this);
	ENSURE(GetSafeHwnd() != NULL);	

	//m_ImageBrowse.DeleteImageList();
	//m_sizeImage = CSize(0, 0);

	OnChangeLayout();
}

void CEditWithBtnCtrl::OnChangeLayout()
{
	ASSERT_VALID(this);
	ENSURE(GetSafeHwnd() != NULL);

	m_nBrowseButtonWidth = max(20, m_sizeImage.cx + 8);

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	
	GetWindowRect(m_rectBtn);
	m_rectBtn.left = m_rectBtn.right -  m_nBrowseButtonWidth;

	ScreenToClient(&m_rectBtn);
	
}

void CEditWithBtnCtrl::OnNcLButtonDblClk(UINT /*nHitTest*/, CPoint /*point*/)
{
}

void CEditWithBtnCtrl::OnBtnClick()
{
	ASSERT_VALID(this);
	ENSURE(GetSafeHwnd() != NULL);

	CWnd* parent = GetParent();
	if(parent)
		parent->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),BN_CLICKED),(LPARAM)GetSafeHwnd());

	SetFocus();
}


void CEditWithBtnCtrl::SetButtonImage(HICON hIcon, BOOL bAutoDestroy)
{
	if (m_ImageBrowse.GetSafeHandle() != NULL)
	{
		m_ImageBrowse.DeleteImageList();
	}

	if (hIcon == NULL)
	{
		m_sizeImage = CSize(0, 0);
		return;
	}

	ICONINFO info;
	::GetIconInfo(hIcon, &info);

	BITMAP bmp;
	::GetObject(info.hbmColor, sizeof(BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = bmp.bmWidth;
	m_sizeImage.cy = bmp.bmHeight;

	::DeleteObject(info.hbmColor);
	::DeleteObject(info.hbmMask);

	UINT nFlags = ILC_MASK;

	switch (bmp.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	m_ImageBrowse.Create(bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);
	m_ImageBrowse.Add(hIcon);	

	if (bAutoDestroy)
	{
		::DestroyIcon(hIcon);
	}
}

void CEditWithBtnCtrl::SetButtonImage(HBITMAP hBitmap, BOOL bAutoDestroy)
{
	if (m_ImageBrowse.GetSafeHandle() != NULL)
	{
		m_ImageBrowse.DeleteImageList();
	}

	if (hBitmap == NULL)
	{
		m_sizeImage = CSize(0, 0);
		return;
	}

	BITMAP bmp;
	::GetObject(hBitmap, sizeof(BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = bmp.bmWidth;
	m_sizeImage.cy = bmp.bmHeight;

	UINT nFlags = ILC_MASK;

	switch (bmp.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	m_ImageBrowse.Create(bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);

	HBITMAP hbmpCopy = (HBITMAP) ::CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, 0);
	m_ImageBrowse.Add(CBitmap::FromHandle(hbmpCopy), RGB(192, 192, 192));

	::DeleteObject(hbmpCopy);

	

	if (bAutoDestroy)
	{
		::DeleteObject(hBitmap);
	}
}

void CEditWithBtnCtrl::SetButtonImage(UINT uiBmpResId)
{
	if (m_ImageBrowse.GetSafeHandle() != NULL)
	{
		m_ImageBrowse.DeleteImageList();
	}

	if (uiBmpResId == 0)
	{
		m_sizeImage = CSize(0, 0);
		return;
	}

	CMFCToolBarImages images;
	if (!images.Load(uiBmpResId))
	{
		ASSERT(FALSE);
		return;
	}

	SetButtonImage((HBITMAP) ::CopyImage(images.GetImageWell(), IMAGE_BITMAP, 0, 0, 0), TRUE /* bAutoDestroy */);
	
}

void CEditWithBtnCtrl::OnAfterUpdate()
{
	if (GetOwner() == NULL)
	{
		return;
	}

	GetOwner()->PostMessage(EN_CHANGE, GetDlgCtrlID(), (LPARAM) GetSafeHwnd());
	GetOwner()->PostMessage(EN_UPDATE, GetDlgCtrlID(), (LPARAM) GetSafeHwnd());
}

void CEditWithBtnCtrl::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	if (!m_bIsButtonCaptured)
	{
		CPoint ptClient = point;
		ScreenToClient(&ptClient);

		if (m_rectBtn.PtInRect(ptClient))
		{
			SetCapture();
			m_bIsButtonHighlighted = TRUE;

			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}

	COXMaskedEdit::OnNcMouseMove(nHitTest, point);
}

void CEditWithBtnCtrl::OnCancelMode()
{
	COXMaskedEdit::OnCancelMode();

	if (IsWindowEnabled())
	{
		ReleaseCapture();
	}

	m_bIsButtonPressed = FALSE;
	m_bIsButtonCaptured = FALSE;
	m_bIsButtonHighlighted = FALSE;

	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

void CEditWithBtnCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rectBtn.PtInRect(point))
	{
		SetFocus();

		m_bIsButtonPressed = TRUE;
		m_bIsButtonCaptured = TRUE;

		SetCapture();

		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		return;
	}

	COXMaskedEdit::OnLButtonDown(nFlags, point);
}

void CEditWithBtnCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_rectBtn.PtInRect(point))
	{
		return;
	}

	COXMaskedEdit::OnRButtonDown(nFlags, point);
}

void CEditWithBtnCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_rectBtn.PtInRect(point))
	{
		return;
	}

	COXMaskedEdit::OnRButtonUp(nFlags, point);
}

BOOL CEditWithBtnCtrl::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
		if ((pMsg->wParam == VK_DOWN || pMsg->wParam == VK_RIGHT))
		{
			OnBtnClick();
			return TRUE;
		}
		break;
	}

	return COXMaskedEdit::PreTranslateMessage(pMsg);
}

LRESULT CEditWithBtnCtrl::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	DWORD dwSize = (DWORD)wParam;
	BYTE* pbInitData = (BYTE*)lParam;
	return 0;
}

BOOL CEditWithBtnCtrl::OnDrawBrowseButton(CDC* pDC, CRect rect, ButtonState state, COLORREF& /*clrText*/)
{
	ASSERT_VALID(pDC);

	pDC->FillRect(&rect, &(GetGlobalData()->brBtnFace));

	CRect rectFrame = rect;
	rectFrame.InflateRect(0, 1, 1, 1);

	pDC->Draw3dRect(rectFrame, GetGlobalData()->clrBtnDkShadow, GetGlobalData()->clrBtnDkShadow);

	rectFrame.DeflateRect(1, 1);
	pDC->DrawEdge(rectFrame, state == ButtonsIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);

	return TRUE;
}