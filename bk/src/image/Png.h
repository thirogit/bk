#ifndef __PNG_H__
#define __PNG_H__

#include "CGdiPlusBitmap.h"

class CPng 
{
public:
	CPng();
	CPng(UINT nIDResource);
	bool LoadPNG(const CString& sPNGPath);
	bool LoadPNG(UINT nIDResource,LPCTSTR pszResourceType = L"PNG");
	HBITMAP GetBitmap24(COLORREF bkgColor) const;
	void Draw(Gdiplus::Graphics& graphics, int posx, int posy);
	CSize GetSize() const;
private:
	CGdiPlusBitmapResource m_PNG;
};
#endif