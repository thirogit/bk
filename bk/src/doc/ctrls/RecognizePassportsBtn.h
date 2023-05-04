#ifndef __RECOGNIZEPASSPORTSBTN_H__
#define __RECOGNIZEPASSPORTSBTN_H__

#include <afxwin.h>
#include "../../ctrls/coolbtn/CoolBtn.h"

#define IDM_RECOGNIZEPASSPORTS_UPDATE (WM_APP+3000)
#define IDM_RECOGNIZEPASSPORTS_CONFIGURE (WM_APP+3001)

class CRecognizePassportsBtn : public CCoolBtn
{
public:
	CRecognizePassportsBtn();
	BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
protected:
	virtual void PreSubclassWindow();
	void CreateMenu();
	
};

#endif