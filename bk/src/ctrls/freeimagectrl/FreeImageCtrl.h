#ifndef __FREEIMAGECTRL_H__
#define __FREEIMAGECTRL_H__

#include "../../image/fipWinImageEx.h"

class CFreeImageCtrl : public CStatic
{
public:
	CFreeImageCtrl();
	virtual bool SetImage(const fipWinImageEx &pix);
	int Create(const RECT &rect,CWnd *pParentWnd);
protected:
	CBitmap m_image;
	
	
	afx_msg void OnPaint();	
	DECLARE_MESSAGE_MAP()	
};

#endif
