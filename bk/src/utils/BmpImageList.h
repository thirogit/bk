#ifndef __BMPIMAGELIST_H__
#define __BMPIMAGELIST_H__

#include <afxwin.h>

class CBmpImageList : public CImageList
{		
public:
	HBITMAP GetBitmap(int imgIndex, COLORREF clrBkGrnd);
};



#endif