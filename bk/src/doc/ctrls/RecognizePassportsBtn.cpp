#include "stdafx.h"
#include "RecognizePassportsBtn.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"

CRecognizePassportsBtn::CRecognizePassportsBtn()
{
}

BOOL CRecognizePassportsBtn::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if(!CCoolBtn::Create(lpszCaption,dwStyle,rect,pParentWnd,nID)) 
		return FALSE;
	CreateMenu();
	return TRUE;
}

void CRecognizePassportsBtn::PreSubclassWindow()
{
	CCoolBtn::PreSubclassWindow();
	CreateMenu();
}

void CRecognizePassportsBtn::CreateMenu()
{
	AddMenuItem(IDM_RECOGNIZEPASSPORTS_UPDATE, StringLoader(IDS_RCGNZEPASS_UPDATE).c_str(), 0);
	AddMenuItem(IDM_RECOGNIZEPASSPORTS_CONFIGURE, StringLoader(IDS_RCGNZEPASS_CONFIGURE).c_str(), 0);	
}

