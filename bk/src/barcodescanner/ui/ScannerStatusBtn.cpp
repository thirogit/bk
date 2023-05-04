#include "stdafx.h"
#include "ScannerStatusBtn.h"
#include "../../Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CScannerStatusBtn, CPngButton)
	
	
	
END_MESSAGE_MAP()

CScannerStatusBtn::CScannerStatusBtn() : m_scannerStatus(ScannerConnectionStatus_Disconnected)
{
}

CScannerStatusBtn::~CScannerStatusBtn()
{

}

void CScannerStatusBtn::SetStatus(ScannerConnectionStatus scannerStatus)
{
	m_scannerStatus = scannerStatus;
	if (::IsWindow(m_hWnd))
		Invalidate(FALSE);
}
	

void CScannerStatusBtn::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	SetWindowText(L"");
	Init();
}

void CScannerStatusBtn::Init()
{
	m_yesBarcodeImg.LoadPNG(IDPNG_YESBARCODE);
	m_noBarcodeImg.LoadPNG(IDPNG_NOBARCODE);
	m_scannerStatus = ScannerConnectionStatus_Disconnected;
}


void CScannerStatusBtn::OnScannerConnected()
{
	SetStatus(ScannerConnectionStatus_Connected);
}

void CScannerStatusBtn::OnScannerDisconnected()
{
	SetStatus(ScannerConnectionStatus_Disconnected);
}

void CScannerStatusBtn::OnBarcode(const std::wstring& sBarcode)
{

}


void CScannerStatusBtn::DrawImage(Gdiplus::Graphics& graphics, const CRect& drawRect)
{
	CPoint center = drawRect.CenterPoint();
	
	CPng* image = NULL;

	if (m_scannerStatus == ScannerConnectionStatus_Connected)
	{
		image = &m_yesBarcodeImg;
	}
	else
	{
		image = &m_noBarcodeImg;
	}

	CSize size = image->GetSize();
	center.Offset(-size.cx / 2, -size.cy / 2);
	image->Draw(graphics, center.x, center.y);
}

	


