#include "stdafx.h"
#include "SectionHeader.h"


CSectionHeader::CSectionHeader()
{	
}

CSectionHeader::~CSectionHeader()
{
}

BEGIN_MESSAGE_MAP(CSectionHeader, CStatic)
	ON_WM_PAINT()
	
//	ON_WM_ERASEBKGND()
	
END_MESSAGE_MAP()

void CSectionHeader::PreSubclassWindow()
{
	
	CStatic::PreSubclassWindow();
}

void CSectionHeader::OnPaint() 
{	
	CPaintDC dc(this);	
    CRect rect;
    GetClientRect(&rect);
	DrawEtchedLine(&dc, rect);


	/*DWORD dwStyle = GetStyle() & 0xFF;
	switch (dwStyle)
	{
	default:
	case SS_LEFT:
		m_eControlAlignment = left;
		break;

	case SS_CENTER:
		m_eControlAlignment = center;
		break;

	case SS_RIGHT:
		m_eControlAlignment = right;
		break;
	}*/

	CRect rectText(rect);

	//if (m_eIconAlignment == right)
	//{
	//	// icon is on right

	//	// there is no spacing if left-aligned 'header'
	//	if ((m_eStyle == groupbox) || (m_eControlAlignment != left))
	//		rectText.left += 3;
	//	rectIcon.left = DrawText(pDC, rectText);
	//	if (m_sizeText.cx != 0)
	//		rectIcon.left += m_nIconSpacing;
	//	DrawIcon(pDC, rectIcon);
	//}
	/*else
	{
	*/	// icon is on left

		// there is no spacing if left-aligned 'header'
	rectText.left += 7;
	
	DrawText(&dc, rectText);
	//}
	
}

CFont * CSectionHeader::GetSafeFont()
{
	// get current font
	CFont *pFont = CWnd::GetFont();

	if (pFont == 0)
	{
		// try to get parent font
		CWnd *pParent = GetParent();
		if (pParent && IsWindow(pParent->m_hWnd))
			pFont = pParent->GetFont();

		if (pFont == 0)
		{
			// no font, so get a system font
			HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
			if (hFont == 0)
				hFont = (HFONT)::GetStockObject(SYSTEM_FONT);
			if (hFont == 0)
				hFont = (HFONT)::GetStockObject(ANSI_VAR_FONT);
			if (hFont)
				pFont = CFont::FromHandle(hFont);
		}
	}

	return pFont;
}

int CSectionHeader::DrawText(CDC *pDC, const CRect& rect)
{
	int rc = rect.left;		

	CString strText = _T("");
	GetWindowText(strText);
	
	if (!strText.IsEmpty())
	{
		COLORREF crText = GetSysColor(COLOR_BTNTEXT);
		if (!IsWindowEnabled())
			crText = GetSysColor(COLOR_GRAYTEXT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));
		pDC->SetBkMode(OPAQUE);

		CFont *pOldFont = pDC->SelectObject(GetSafeFont());

		// always left aligned within drawing rect
		UINT nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;

		CRect rectText(rect);
		// get size of text so we can update rect
		CSize size = pDC->GetTextExtent(strText);
		rectText.right = rectText.left + size.cx;

		pDC->DrawText(strText, rectText, nFormat);

		rc = rectText.right;

		if (pOldFont)
			pDC->SelectObject(pOldFont);
	}

	return rc;
}

void CSectionHeader::DrawEtchedLine(CDC *pDC, const CRect& rect)
{
	COLORREF crDark = GetSysColor(COLOR_BTNSHADOW);
	CPen penDark(PS_SOLID, 1, crDark);
	CPen *pOldPen = pDC->SelectObject(&penDark);

	int y = rect.top + rect.Height() / 2;

	pDC->MoveTo(rect.left, y);
	pDC->LineTo(rect.right, y);

	
	CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT));
	pDC->SelectObject(&penLight);
	pDC->MoveTo(rect.left, y + 1);
	pDC->LineTo(rect.right, y + 1);
	

	if (pOldPen)
		pDC->SelectObject(pOldPen);
}
