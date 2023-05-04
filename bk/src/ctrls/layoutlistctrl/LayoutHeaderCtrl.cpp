#include "stdafx.h"
#include "LayoutHeaderCtrl.h"

CLayoutHeaderCtrl::CLayoutHeaderCtrl()
{
}

CLayoutHeaderCtrl::~CLayoutHeaderCtrl()
{
}

BEGIN_MESSAGE_MAP(CLayoutHeaderCtrl,CHeaderCtrl)
     ON_MESSAGE(HDM_LAYOUT, OnHeaderLayout)
	 ON_WM_CREATE()
END_MESSAGE_MAP()

LRESULT CLayoutHeaderCtrl::OnHeaderLayout(WPARAM, LPARAM lParam)
{
     LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

     LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

     lphdlayout->pwpos->cy = 25;
     lphdlayout->prc->top = 25;

     return lResult;
}


int CLayoutHeaderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int createResult = CHeaderCtrl::OnCreate(lpCreateStruct);
	if(createResult != 0) return createResult;

	
	

	return 0;
}
