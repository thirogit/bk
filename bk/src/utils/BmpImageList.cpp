#include "stdafx.h"
#include "BmpImageList.h"

HBITMAP CBmpImageList::GetBitmap(int imgIndex, COLORREF clrBkGrnd)
{
  if (imgIndex < 0)
    return NULL;
 
  IMAGEINFO info = {0};
  GetImageInfo(imgIndex, &info);
 
  CSize imageSize(info.rcImage.right - info.rcImage.left,
                  info.rcImage.bottom - info.rcImage.top);
   
  CDC memDC;	
  HDC hScreenDC = ::GetDC(NULL);
  memDC.Attach(::CreateCompatibleDC(hScreenDC));
  ::ReleaseDC(NULL,hScreenDC);

  UINT w = info.rcImage.right - info.rcImage.left;
  UINT h = info.rcImage.bottom - info.rcImage.top;

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

	memDC.FillSolidRect(0, 0, w, h, clrBkGrnd);
	
	Draw(&memDC,imgIndex,CPoint(0,0),ILD_NORMAL);
	
 	memDC.SelectObject(pOldMemDCBmp);
	memDC.DeleteDC();
	bmp.Detach();

	return hDIBBmp;
}
