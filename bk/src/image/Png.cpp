#include "stdafx.h"
#include "Png.h"


CPng::CPng()
{

}

CPng::CPng(UINT nIDResource)
{
	LoadPNG(nIDResource);
}

bool CPng::LoadPNG(const CString& sPNGPath)
{
	m_PNG.Empty();
	//DWORD dwPathLen = MultiByteToWideChar (CP_ACP, 0, sPNGPath, -1, NULL, 0);
	//wchar_t *pwText  = new wchar_t[dwPathLen+1];
	//MultiByteToWideChar (CP_ACP, 0, sPNGPath, -1, pwText, dwPathLen );
	bool bResult = m_PNG.LoadFile(sPNGPath);
	//delete []pwText;
	return bResult;	
}

bool CPng::LoadPNG(UINT nIDResource,LPCTSTR pszResourceType/* = "PNG"*/)
{
	m_PNG.Empty();
	return m_PNG.Load(nIDResource,pszResourceType,AfxGetInstanceHandle());
}

HBITMAP CPng::GetBitmap24(COLORREF bkgColor) const
{
	if(!m_PNG.m_pBitmap)
		return NULL;

	CDC memDC;	
	HDC hScreenDC = ::GetDC(NULL);
	memDC.Attach(::CreateCompatibleDC(hScreenDC));
	::ReleaseDC(NULL,hScreenDC);

	UINT w = m_PNG.m_pBitmap->GetWidth();
	UINT h = m_PNG.m_pBitmap->GetHeight();

	BITMAPINFO bmInfo;
	memset(&bmInfo.bmiHeader,0,sizeof(BITMAPINFOHEADER));
	bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth=w;
	bmInfo.bmiHeader.biHeight= h;
	bmInfo.bmiHeader.biPlanes=1;
	bmInfo.bmiHeader.biBitCount=24;
	BYTE *pbase;
	HBITMAP hDIBBmp = CreateDIBSection(memDC.GetSafeHdc(), &bmInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);

	CBitmap bmp;
	bmp.Attach(hDIBBmp);
	CBitmap* pOldMemDCBmp = memDC.SelectObject(&bmp);

	CBrush brush(bkgColor);
	memDC.FillRect(CRect(0, 0, w, h), &brush);

	Gdiplus::Graphics graphics(memDC.GetSafeHdc());
	graphics.DrawImage(m_PNG.m_pBitmap,0,0,w,h);
 	memDC.SelectObject(pOldMemDCBmp);
	memDC.DeleteDC();
	bmp.Detach();
	return hDIBBmp;
}

void CPng::Draw(Gdiplus::Graphics& graphics, int posx, int posy)
{
	graphics.DrawImage(m_PNG.m_pBitmap, posx, posy, m_PNG.m_pBitmap->GetWidth(), m_PNG.m_pBitmap->GetHeight());
}

CSize CPng::GetSize() const
{
	return CSize(m_PNG.m_pBitmap->GetWidth(), m_PNG.m_pBitmap->GetHeight());
}


//HBITMAP CPng::GetBitmap32()
//{
//	if(!m_PNG.m_pBitmap)
///		return NULL;

//    HBITMAP hBitmap = NULL;

	//UINT w = m_PNG.m_pBitmap->GetWidth();
	//UINT h = m_PNG.m_pBitmap->GetHeight();

	//BITMAPINFO bmInfo;
	//memset(&bmInfo.bmiHeader,0,sizeof(BITMAPINFOHEADER));
	//bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	//bmInfo.bmiHeader.biWidth=w;
	//bmInfo.bmiHeader.biHeight= h;
	//bmInfo.bmiHeader.biPlanes=1;
	//bmInfo.bmiHeader.biBitCount=32;
	//BYTE *ppvBits = NULL;

	//HBITMAP hDIBBmp = CreateDIBSection(NULL, &bmInfo,DIB_RGB_COLORS,(void**)&ppvBits,0,0);
	//
	//BITMAPINFO* pbmi = (BITMAPINFO*)ppvBits;
 //  
 //  // bm.RotateFlip( RotateFlipType.RotateNoneFlipY );
 //   // Required due to the way bitmap is copied and read

	//BitmapData* pBmData = NULL;
	//Gdiplus::Rect rect(0,0,w,h);
	//m_PNG.m_pBitmap->LockBits(&rect,Gdiplus::ImageLockModeRead,Gdiplus::PixelFormat32bppARGB,&pBmData);

	//memcpy(ppvBits,pBmData->Scan0,h*pBmData->Stride);

	//m_PNG.m_pBitmap->UnlockBits(pBmData);

//	return NULL;
//}