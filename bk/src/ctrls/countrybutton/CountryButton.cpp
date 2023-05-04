#include "stdafx.h"
#include "CountryButton.h"


BEGIN_MESSAGE_MAP(CCountryButton, CButton)

	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	

END_MESSAGE_MAP()

CCountryButton::CCountryButton() 
{
}

CCountryButton::~CCountryButton()
{
}

void CCountryButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
	CButton::PreSubclassWindow();
}

void CCountryButton::OnDestroy()
{
	DestroyFlagIcon();
	CButton::OnDestroy();	
}

void CCountryButton::DestroyFlagIcon()
{
	m_flagIcon.DeleteObject();
}

void CCountryButton::SetCountry(const Country& country)
{
	DestroyFlagIcon();

	m_Country = country;
	if (!m_Country.IsNull())
	{
		m_flagIcon.Attach(m_flagsIL.GetFlagBitmap(m_Country.GetCountryISONumber(), ::GetSysColor(COLOR_3DFACE)));
	}
	else 
	{
		DestroyFlagIcon();
	}
}

const Country& CCountryButton::GetCountry() const
{
	return m_Country;
}

BOOL CCountryButton::OnEraseBkgnd(CDC* pDC)
{
    CRect rectClient;        // Rectangle for entire client area
    GetClientRect(&rectClient);

    // Fill the rectangle
    //pDC->FillSolidRect(&rectClient, m_crColor);

	return TRUE;
}


void CCountryButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	

	if (!m_Country.IsNull())
	{
		int iconWidth = 16;
		int iconHeight = 16;
		int flagY = rDraw.top + ((rDraw.Height() - iconHeight) / 2);
				
		CDC memDC;		
		memDC.Attach(::CreateCompatibleDC(pDC->GetSafeHdc()));
		CBitmap* pOldMemDCBmp = memDC.SelectObject(&m_flagIcon);

		pDC->BitBlt(rDraw.left, flagY, iconWidth, iconHeight, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldMemDCBmp);
		memDC.DeleteDC();
				
		std::wstring countryCode2a = m_Country.GetCountryCode2A();

		CSize size = pDC->GetTextExtent(countryCode2a.c_str(), countryCode2a.length());

		int txtY = rDraw.top + ((rDraw.Height() - size.cy) / 2);
		int txtX = rDraw.left + iconWidth + 4;

		pDC->DrawText(countryCode2a.c_str(), countryCode2a.length(), CRect(txtX, txtY, txtX + size.cx, txtY + size.cy), DT_SINGLELINE);
	}

}
