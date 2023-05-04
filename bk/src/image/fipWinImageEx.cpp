#include "stdafx.h"
#include "fipWinImageEx.h"
#include "freeimage.h"

#include <afxole.h>

fipWinImageEx::fipWinImageEx(FREE_IMAGE_TYPE image_type/* = FIT_BITMAP*/, WORD width/* = 0*/, WORD height/* = 0*/, WORD bpp/* = 0*/) :
				fipWinImage(image_type,width,height,bpp)
{
}

fipWinImageEx::fipWinImageEx(const fipWinImage& src) 
{
	fipWinImage::operator =(src);
}

fipWinImageEx& fipWinImageEx::operator=(const fipWinImageEx& right)
{
	fipWinImage::operator =(right);
	return *this;
}

fipWinImageEx::fipWinImageEx(const fipWinImageEx&& src)
{
	this->_dib = src._dib;
}

BOOL fipWinImageEx::copyFromHDIB(HANDLE hMem) {
	BYTE *lpVoid = NULL;
	BITMAPINFOHEADER *pHead = NULL;
	RGBQUAD *pPalette = NULL;
	BYTE *bits = NULL;
	DWORD bitfields[3] = {0, 0, 0};
	DWORD fiBugOffset = 0;
	// Get a pointer to the bitmap
	lpVoid = (BYTE *)GlobalLock(hMem);

	// Get a pointer to the bitmap header
	pHead = (BITMAPINFOHEADER *)lpVoid;

	if(sizeof(BITMAPINFOHEADER) != pHead->biSize) return FALSE;

	WORD wBpp = WORD(pHead->biPlanes * pHead->biBitCount);

	if( wBpp <= 1 )
		wBpp = 1;
	else if( wBpp <= 2 )
		wBpp = 2;
	else if( wBpp <= 4 )
		wBpp = 4;
	else if( wBpp <= 8 )
		wBpp = 8;
	else if( wBpp <= 16 )
		wBpp = 16;
	else if( wBpp <= 24 )
		wBpp = 24;
	else
		wBpp = 32;

	// Get a pointer to the palette
	ASSERT(wBpp < 16);

	pPalette = (RGBQUAD *)(((BYTE *)pHead) + sizeof(BITMAPINFOHEADER));


	DWORD dwMaxColor =0,dwNumColors = 0;
	if(pHead->biClrUsed)
		dwNumColors = pHead->biClrUsed;
	else
	{
		dwMaxColor = 1 << wBpp;
		dwNumColors = dwMaxColor;
		pHead->biClrUsed = dwNumColors; //repair , to avoid FreeImage library bug

	}
	// Get a pointer to the pixels
	bits = ((BYTE*)pHead + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dwNumColors);

	if(pHead->biCompression == BI_BITFIELDS) {
		// Take into account the color masks that specify the red, green, and blue components (16- and 32-bit)
		unsigned mask_size = 3 * sizeof(DWORD);
		memcpy(&bitfields[0], bits, mask_size);
		bits += mask_size;
	} 

	if(lpVoid) {

		// Allocate a new FIBITMAP

		FREE_IMAGE_TYPE image_type = FIT_BITMAP;
		// Use a hack to decide if the clipboard contains non standard bitmaps ...
		switch(pHead->biCompression - 0xFF)
		{
			case FIT_UINT16:
			case FIT_INT16:
			case FIT_UINT32:
			case FIT_INT32:
			case FIT_FLOAT:
			case FIT_DOUBLE:
			case FIT_COMPLEX:
			case FIT_RGB16:
			case FIT_RGBA16:
			case FIT_RGBF:
			case FIT_RGBAF:
				image_type = (FREE_IMAGE_TYPE)(pHead->biCompression - 0xFF);
				break;
		}

		if(!setSize(image_type, (WORD)pHead->biWidth, (WORD)pHead->biHeight, pHead->biBitCount, bitfields[2], bitfields[1], bitfields[0])) 
		{
			GlobalUnlock(lpVoid);
			return FALSE;
		}

		// Copy the bitmap header
		memcpy(FreeImage_GetInfoHeader(_dib), pHead, sizeof(BITMAPINFOHEADER));

		memcpy(FreeImage_GetPalette(_dib), pPalette, dwNumColors * sizeof(RGBQUAD));

		// Copy the bitmap
		memcpy(((BYTE*)FreeImage_GetBits(_dib))+fiBugOffset, bits, FreeImage_GetPitch(_dib) * FreeImage_GetHeight(_dib));
		
		GlobalUnlock(lpVoid);

		return TRUE;
	}

	return FALSE;
}



bool fipWinImageEx::copyToBitmap(CBitmap &bitmap,CDC *pDC) const 
{
	if(_dib)	
	{

		if(FreeImage_GetImageType(_dib) != FIT_BITMAP) return false;

		const BITMAPINFOHEADER& bih = *FreeImage_GetInfoHeader(_dib);;

		if (bitmap.m_hObject) bitmap.DeleteObject();

		if (!bitmap.CreateCompatibleBitmap(pDC, bih.biWidth, bih.biHeight))
			return false;

		bitmap.SetBitmapDimension(bih.biWidth, bih.biHeight);
			
	const BYTE* pData = FreeImage_GetBits(_dib);
		//(BYTE*)pDIB->bmiColors + CalcPaletteSize(bih.biBitCount);

	BITMAPINFO* pDIB = (BITMAPINFO*)getInfo();

	return SetDIBits(pDC->GetSafeHdc(), bitmap, 0, bih.biHeight, pData, pDIB, DIB_RGB_COLORS) > 0;
	}

	return false;

}

UINT DLL_CALLCONV fipWinImageEx::FI_ReadProcNFS (void *buffer, unsigned size, unsigned count, fi_handle handle)
{

	COleStreamFile *out = (COleStreamFile*)handle;
	return out->Read(buffer,size*count);
}
UINT DLL_CALLCONV fipWinImageEx::FI_WriteProcNFS (void *buffer, unsigned size, unsigned count, fi_handle handle)
{

	COleStreamFile *out = (COleStreamFile*)handle;
	out->Write(buffer,size*count);
	return size*count;

}
int DLL_CALLCONV fipWinImageEx::FI_SeekProcNFS (fi_handle handle, long offset, int origin)
{
	COleStreamFile *out = (COleStreamFile*)handle;

	CFile::SeekPosition nFrom;
	switch(origin)
	{
	case SEEK_SET:
		nFrom = CFile::begin;
	break;
	case SEEK_CUR:
		nFrom = CFile::current;
	break;
	case SEEK_END:
		nFrom = CFile::end;
	break;
	default:
		ASSERT(0);
	break;
	}
	return (int)out->Seek(offset,nFrom);
}

long DLL_CALLCONV fipWinImageEx::FI_TellProcNFS (fi_handle handle)
{
	COleStreamFile *out = (COleStreamFile*)handle;
	return (int)out->GetPosition();
}
