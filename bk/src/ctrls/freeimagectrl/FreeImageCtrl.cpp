#include "stdafx.h"
#include "FreeImageCtrl.h"

BEGIN_MESSAGE_MAP(CFreeImageCtrl,CStatic)
	ON_WM_PAINT()

END_MESSAGE_MAP()

CFreeImageCtrl::CFreeImageCtrl() 
{
}


int CFreeImageCtrl::Create(const RECT &rect,CWnd *pParentWnd)
{
	return CStatic::Create(L"",WS_CHILD,rect,pParentWnd);
}

bool CFreeImageCtrl::SetImage(const fipWinImageEx &pix)
{
	bool bRet;
	if (m_image.m_hObject) m_image.DeleteObject();
	
	CDC *pDC = GetDC();
	bRet = pix.copyToBitmap(m_image, pDC);
	ReleaseDC(pDC);
	Invalidate();
	
	return bRet;
}

void CFreeImageCtrl::OnPaint()
{
	CStatic::OnPaint();
	CClientDC dc(this);

	if (m_image.m_hObject)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = memDC.SelectObject(&m_image);
		CSize bmpSize = m_image.GetBitmapDimension();
		CRect rc;
		GetClientRect(&rc);

		if (bmpSize.cx < rc.Width() && bmpSize.cy < rc.Height())
		{
				dc.BitBlt(0, 0, bmpSize.cx, bmpSize.cy, &memDC, 0, 0, SRCCOPY);
		}
		else
		{
				double scale = min(double(rc.Width()) / bmpSize.cx, double(rc.Height()) / bmpSize.cy);
				dc.StretchBlt(0, 0 ,int(bmpSize.cx*scale), int(bmpSize.cy*scale), &memDC, 0, 0, bmpSize.cx, bmpSize.cy, SRCCOPY);
		}
		memDC.SelectObject(pOldBitmap);
	}
}




