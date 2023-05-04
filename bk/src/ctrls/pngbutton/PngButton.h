#ifndef __PNGBUTTON_H__
#define __PNGBUTTON_H__

#include "../../image/Png.h"

class CPngButton : public CButton
{
public:
	CPngButton();
	virtual ~CPngButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetImage(UINT nResPNG);

protected:
	virtual void PreSubclassWindow();
	virtual void DrawImage(Gdiplus::Graphics& graphics,const CRect& drawRect);

protected:
	
	CPng m_image;
	
protected:
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	

	DECLARE_MESSAGE_MAP()
};


#endif 
