#include "stdafx.h"
#include "ScannerStatusIndicator.h"
#include "../../Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CScannerStatusIndicator, CStatic)
	
	ON_WM_PAINT()
	ON_WM_ENABLE()
	
END_MESSAGE_MAP()

CScannerStatusIndicator::CScannerStatusIndicator() : m_scannerStatus(ScannerConnectionStatus_Disconnected)
{
}

CScannerStatusIndicator::~CScannerStatusIndicator()
{

}

void CScannerStatusIndicator::SetStatus(ScannerConnectionStatus scannerStatus)
{
	m_scannerStatus = scannerStatus;
	if (::IsWindow(m_hWnd))
		Invalidate(FALSE);
}
	

void CScannerStatusIndicator::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	SetWindowText(L"");
	Init();
}

void CScannerStatusIndicator::Init()
{
	m_yesBarcodeImg.LoadPNG(IDPNG_YESBARCODE);
	m_noBarcodeImg.LoadPNG(IDPNG_NOBARCODE);
	m_yesBarcodeDisabledImg.LoadPNG(IDPNG_YESBARCODEDISABLED);
	m_noBarcodeDisabledImg.LoadPNG(IDPNG_NOBARCODEDISABLED);
	m_scannerStatus = ScannerConnectionStatus_Disconnected;
}


void CScannerStatusIndicator::OnPaint()
{
	CStatic::OnPaint();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc.m_hDC);

	CRect client;
	GetClientRect(&client);

	CPng* indicatorImage = NULL;
	BOOL enabled = IsWindowEnabled();
	if (m_scannerStatus == ScannerConnectionStatus_Connected)
	{		
		indicatorImage = enabled ? &m_yesBarcodeImg : &m_yesBarcodeDisabledImg;
	}
	else
	{
		indicatorImage = enabled ? &m_noBarcodeImg : &m_noBarcodeDisabledImg;
	}

	CSize imageSize = indicatorImage->GetSize();

	int x = max((client.Width() - imageSize.cx) / 2, 0);
	int y = max((client.Height() - imageSize.cy) / 2, 0);

	indicatorImage->Draw(graphics, x, y);
	graphics.ReleaseHDC(dc.m_hDC);
}

void CScannerStatusIndicator::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

	/*m_pFrom->EnableWindow(bEnable);
	m_pSeparator->EnableWindow(bEnable);
	m_pTo->EnableWindow(bEnable);



	SendMessage(WM_SETREDRAW, TRUE, 0);
	

*/

}


