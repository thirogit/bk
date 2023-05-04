#include "stdafx.h"
#include "HentBtn.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"
#include "../../utils/TempIconLoader.h"

const int nDropBtnWidth =  16;

#define IDM_NONEHENT 1001
#define IDM_EDITHENT 1002

CHentBtn::CHentBtn()
{
	m_hentType		= HentType_None;
	m_hentId		= NULL_ID;
	m_sNoneHent		= StringLoader(IDS_HENT_NONE);
	m_bPushed		= FALSE;
	m_bMenuPushed	= FALSE;
	m_IsMenuEnabled = TRUE;
	
}

CHentBtn::~CHentBtn()
{
}


BEGIN_MESSAGE_MAP(CHentBtn, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(IDM_NONEHENT,OnNoHent)
	ON_COMMAND(IDM_EDITHENT,OnEditHent)
	ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

BOOL CHentBtn::Create(  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	dwStyle |= BS_OWNERDRAW; // Enforce
	m_pParentWnd = pParentWnd;
	if(CButton::Create(L"", dwStyle, rect, pParentWnd, nID ) == 0)
	{
		LoadHentImages();		
		return 0;
	}

	return -1;
}

void CHentBtn::OnStyleChanged(int styleType, LPSTYLESTRUCT style)
{
	if (GWL_EXSTYLE == styleType)
	{
		if ((style->styleNew & HBS_EX_NOEDITHENT) ^ (style->styleOld & HBS_EX_NOEDITHENT))
		{
			if (m_menu.m_hMenu)
			{
				m_menu.DestroyMenu();
			}
			CreateMenu();
		}
	}
}

void CHentBtn::CreateMenu()
{
	if (m_menu.m_hMenu) return;
	

	m_menu.CreatePopupMenu();
	
	MENUITEMINFO menuItem;
	memset(&menuItem,0,sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
		

	std::wstring sSetHentToNoneMenuItemtext = StringLoader(IDS_SETHENTTONONE);
	menuItem.fMask =  MIIM_STRING | MIIM_ID | MIIM_BITMAP;
	menuItem.dwTypeData = const_cast<wchar_t*>(sSetHentToNoneMenuItemtext.c_str());
	menuItem.cch = m_sNoneHent.length();
	menuItem.wID = IDM_NONEHENT;
	menuItem.hbmpItem = m_IL.GetBitmap(0,GetSysColor(COLOR_MENU));
	
	m_menu.InsertMenuItem(0,&menuItem,TRUE);	

	DWORD exStyle = GetExStyle();
	if (!(exStyle & HBS_EX_NOEDITHENT))
	{
		std::wstring sEditHentMenuItemtext = StringLoader(IDS_EDITHENT);
		menuItem.fMask = MIIM_STRING | MIIM_ID;
		menuItem.dwTypeData = const_cast<wchar_t*>(sEditHentMenuItemtext.c_str());
		menuItem.cch = m_sNoneHent.length();
		menuItem.wID = IDM_EDITHENT;
		menuItem.hbmpItem = m_IL.GetBitmap(0, GetSysColor(COLOR_MENU));

		m_menu.InsertMenuItem(1, &menuItem, TRUE);
	}
}

void CHentBtn::LoadHentImages()
{
	m_IL.DeleteImageList();
	m_IL.Create(16,16,ILC_COLOR8|ILC_MASK,3,1);	
	m_IL.SetBkColor(CLR_NONE);
	m_IL.Add(TempIconLoader(IDI_HENTBLUE));
	m_IL.Add(TempIconLoader(IDI_HENTRED));
	m_IL.Add(TempIconLoader(IDI_HENTGREEN));
}

uint32_t CHentBtn::GetHent() const
{
	return m_hentId;
}

int CHentBtn::GetImageForHentType(HentType type)
{
	switch(type)
	{
	case HentType_None:
		return 0;
	case HentType_Company:
		return 1;
	case HentType_Individual:
		return 2;
	}
	return -1;
}

//void CHentBtn::DrawMasked() const
//{
//
//	if (himl)
//	{
//		INT cx, cy;
//		HBITMAP hbmMask, hbmImage;
//		HDC hdcMask, hdcImage;
//
//		ImageList_GetIconSize(himl, &cx, &cy);
//
//		/* Create src image */
//		hdcImage = CreateCompatibleDC(hdc);
//		hbmImage = CreateBitmap(cx, cy, GetDeviceCaps(hdc, PLANES),
//			GetDeviceCaps(hdc, BITSPIXEL), NULL);
//		SelectObject(hdcImage, hbmImage);
//		ImageList_DrawEx(himl, btnPtr->iBitmap, hdcImage, 0, 0, cx, cy,
//			RGB(0xff, 0xff, 0xff), RGB(0, 0, 0), ILD_NORMAL);
//
//		/* Create Mask */
//		hdcMask = CreateCompatibleDC(0);
//		hbmMask = CreateBitmap(cx, cy, 1, 1, NULL);
//		SelectObject(hdcMask, hbmMask);
//
//		/* Remove the background and all white pixels */
//		SetBkColor(hdcImage, ImageList_GetBkColor(himl));
//		BitBlt(hdcMask, 0, 0, cx, cy, hdcImage, 0, 0, SRCCOPY);
//		SetBkColor(hdcImage, RGB(0xff, 0xff, 0xff));
//		BitBlt(hdcMask, 0, 0, cx, cy, hdcImage, 0, 0, NOTSRCERASE);
//
//		/* draw the new mask 'etched' to hdc */
//		SetBkColor(hdc, RGB(255, 255, 255));
//		SelectObject(hdc, GetSysColorBrush(COLOR_3DHILIGHT));
//		BitBlt(hdc, x + 1, y + 1, cx, cy, hdcMask, 0, 0, 0xE20746);
//		SelectObject(hdc, GetSysColorBrush(COLOR_3DSHADOW));
//		BitBlt(hdc, x, y, cx, cy, hdcMask, 0, 0, 0xE20746);
//
//		/* Cleanup */
//		DeleteObject(hbmImage);
//		DeleteDC(hdcImage);
//		DeleteObject(hbmMask);
//		DeleteDC(hdcMask);
//	}
//}

void CHentBtn::DrawItem(DRAWITEMSTRUCT* lpDIS) 
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
	pDC->FillRect(trueRect,&CBrush(GetSysColor(COLOR_BTNFACE)));
	
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
		pDC->DrawFrameControl(&btnRect,DFC_BUTTON,DFCS_BUTTONPUSH);


	////////////////////////////////////////
	// Default Button State               //
	////////////////////////////////////////
	if ( m_bPushed && !bDisabled)
	{
		pDC->FrameRect(&lpDIS->rcItem,CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
	
		if (m_bPushed && !m_bMenuPushed)
			pDC->FrameRect(&btnRect,CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH))); 
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
	int imgY = (rectContent.Height() - 16) / 2;
	int imgIndex = GetImageForHentType(m_hentType);
	const int marginX = 3;
	if (!bDisabled)
	{
				
		m_IL.Draw(pDC, imgIndex,CPoint(marginX+rectContent.left+m_bPushed,imgY+m_bPushed), ILD_NORMAL);
	}
	else
	{
		IMAGEINFO ii;
		m_IL.GetImageInfo(imgIndex, &ii);
		HBITMAP img = m_IL.GetBitmap(imgIndex, RGB(255,255,255));
		pDC->DrawState(CPoint(marginX+rectContent.left +m_bPushed, imgY +m_bPushed), CSize(ii.rcImage.right-ii.rcImage.left, ii.rcImage.bottom-ii.rcImage.top), img, DST_BITMAP | DSS_DISABLED);
		DeleteObject(img);
	}
	


	////////////////////////////////////////
	// Draw out text                      //
	////////////////////////////////////////
	pDC->SelectObject(pFont);
	CRect rectText(rectContent);
	rectText.left += (16+ marginX + 1);
	
	std::wstring sCaption = m_sNoneHent;
	if(m_hentId != NULL_ID)
		sCaption = m_sContentText;
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));

	if (bDisabled)
	{
		rectText.OffsetRect(1,1);
		pDC->SetTextColor(GetSysColor(COLOR_WINDOW));
		pDC->DrawText(sCaption.c_str(),sCaption.length(),rectText,DT_SINGLELINE|DT_CENTER| DT_VCENTER);

		rectText.OffsetRect(-1,-1);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->DrawText(sCaption.c_str(),sCaption.length(),rectText,DT_SINGLELINE|DT_CENTER| DT_VCENTER);
	}
	else
	{
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pDC->DrawText(sCaption.c_str(),sCaption.length(),rectText,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}


	CRect rectSplit(btnRect);
	rectSplit.DeflateRect(2,2);
	rectSplit.right -= nDropBtnWidth;


	////////////////////////////////////////
	// Drop down split                    //
	////////////////////////////////////////
	CPen brFace(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
	pDC->SelectObject(&brFace);
	pDC->MoveTo(rectSplit.right, rectSplit.top);
	pDC->LineTo(rectSplit.right, rectSplit.bottom);


	CPen brLite(PS_SOLID,1,GetSysColor(COLOR_3DHILIGHT));
	pDC->SelectObject(&brLite);
	pDC->MoveTo(rectSplit.right+1 , rectSplit.top);
	pDC->LineTo(rectSplit.right+1, rectSplit.bottom);


	rectSplit.left = rectSplit.right;
	rectSplit.right += nDropBtnWidth;

	CPoint pt(rectSplit.CenterPoint());
	pt += CPoint(m_bPushed,m_bPushed);

	CPen penBlack(PS_SOLID, 1, (bDisabled || !m_IsMenuEnabled) ? GetSysColor(COLOR_GRAYTEXT) : GetSysColor(COLOR_WINDOWTEXT));
	pDC->SelectObject(&penBlack);
	DrawArrow(pDC,pt);

	////////////////////////////////////////
	// Drop down state                    //
	////////////////////////////////////////
	if (m_bMenuPushed && !bDisabled)
	{    
		rectSplit.InflateRect(1,1);
		pDC->DrawEdge(rectSplit,BDR_SUNKENOUTER, BF_RECT);
	}
}

void CHentBtn::DrawArrow(CDC* pDC,CPoint ArrowTip)
{

	CPoint ptDest;

	CPen* pPen = pDC->GetCurrentPen();
	LOGPEN logPen;

//	CPen newPen(PS_SOLID,1,RGB(
	pPen->GetLogPen(&logPen);
	pDC->SetPixel(ArrowTip, logPen.lopnColor);


	ArrowTip -= CPoint(1,1);
	pDC->MoveTo(ArrowTip);

	ptDest = ArrowTip;
	ptDest += CPoint(3,0);
	pDC->LineTo(ptDest);

	ArrowTip -= CPoint(1,1);
	pDC->MoveTo(ArrowTip);

	ptDest = ArrowTip;
	ptDest += CPoint(5,0);
	pDC->LineTo(ptDest);

	ArrowTip -= CPoint(1,1);
	pDC->MoveTo(ArrowTip);

	ptDest = ArrowTip;
	ptDest += CPoint(7,0);
	pDC->LineTo(ptDest);
}


void CHentBtn::OnLButtonDown(UINT nFlags, CPoint point) 
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

		int x = rc.right-nDropBtnWidth;
		int y = rc.bottom;

		CreateMenu();

		m_menu.EnableMenuItem(IDM_EDITHENT, MF_BYCOMMAND | (m_hentId != NULL_ID ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));

		m_menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,x,y,this);

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

void CHentBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{

	if (m_bPushed)
	{
		m_bPushed = FALSE;
		ReleaseCapture();
		Invalidate();
		m_pParentWnd->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM) GetSafeHwnd());
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CHentBtn::OnMouseMove(UINT nFlags, CPoint point) 
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

void CHentBtn::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	Invalidate();	
}

void CHentBtn::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);	
}

void CHentBtn::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	Invalidate();	
}
 
BOOL CHentBtn::HitMenuBtn(CPoint point)
{
	ClientToScreen(&point);

	CRect rect;
	GetWindowRect(rect);
	rect.left = rect.right - nDropBtnWidth;

	return rect.PtInRect(point);    
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	  AddMenuItem
//
// DESCRIPTION:	Adds a menu item and id to our menu.
//
// NOTES:			
//
// MAINTENANCE:
// Name:		  Date:	  Version:	Notes:
// NT ALMOND	210100	1.0			  Origin
//
////////////////////////////////////////////////////////////////////////////////
//BOOL CHentBtn::AddMenuItem(UINT nMenuId,const CString& sItemText, UINT nMenuFlags)
//{
//	BOOL bRet = m_menu.AppendMenu(nMenuFlags | MF_STRING, nMenuId, (LPCTSTR)sItemText);
//
//	m_bMenuLoaded |= bRet;
//
//	return bRet;
//}

void CHentBtn::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW); // Enforce
	m_pParentWnd = GetParent();
	CButton::PreSubclassWindow();
	LoadHentImages();
	

}

//void CHentBtn::EnableMenu(BOOL bEnable)
//{
//	m_IsMenuEnabled = bEnable;
//	Invalidate(0);
//}

void CHentBtn::InitHent(uint32_t hentId)
{
	m_hentType = HentType_None;
	m_hentId = hentId;
	if (m_hentId != NULL_ID)
	{
		SeasonSession* session = GetSeasonSession();

		HentClientObject* pHentCO = session->GetHent(m_hentId);
		if (pHentCO != NULL)
		{
			IHent* pHent = pHentCO->Object();
			m_sContentText = pHent->GetAlias();
			m_hentType = pHent->GetHentType();
		}
		else
		{
			m_hentId = NULL_ID;
		}
	}
}

void CHentBtn::SetHent(uint32_t hentId)
{	
	InitHent(hentId);

	NMHDR hdr;
	hdr.code = BN_HENTCHANGE;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();
	GetParent()->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
	
	if(m_hWnd != NULL)
		Invalidate(FALSE);

}

void CHentBtn::OnNoHent()
{
	SetHent(NULL_ID);
}

void CHentBtn::OnEditHent()
{
	NMHDR hdr;
	hdr.code = BN_EDITHENT;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();
	GetParent()->SendMessage(WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr);

}