#ifndef __FIPWINIMAGEEX_H__
#define __FIPWINIMAGEEX_H__

#include "FreeImagePlus.h"

class fipWinImageEx : public fipWinImage
{
public:
	fipWinImageEx(FREE_IMAGE_TYPE image_type = FIT_BITMAP, WORD width = 0, WORD height = 0, WORD bpp = 0);
	fipWinImageEx(const fipWinImage& src);
	fipWinImageEx& operator=(const fipWinImageEx& right);
	fipWinImageEx(const fipWinImageEx&& src);
	bool copyToBitmap(CBitmap &bitmap,CDC *pDC) const ;
	BOOL copyFromHDIB(HANDLE hMem);

	static UINT DLL_CALLCONV  FI_ReadProcNFS (void *buffer, unsigned size, unsigned count, fi_handle handle);
	static UINT DLL_CALLCONV FI_WriteProcNFS (void *buffer, unsigned size, unsigned count, fi_handle handle);
	static int DLL_CALLCONV FI_SeekProcNFS (fi_handle handle, long offset, int origin);
	static long DLL_CALLCONV FI_TellProcNFS (fi_handle handle);

	
	
};


#endif // __FIWINIMAGE_H__
