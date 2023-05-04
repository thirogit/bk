#include "stdafx.h"
#include "DecimalRangeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DestroyControl(ctrl) \
	if (ctrl != NULL) { ctrl->DestroyWindow(); delete ctrl; ctrl = NULL; }


enum { ID_FROM = 3333, ID_LABEL, ID_TO };


CDecimalRangeCtrl::CDecimalRangeCtrl(int precision /*= 6*/) : CStatic(),m_pFrom(NULL), m_pTo(NULL), m_pSeparator(NULL), m_precision(precision)
{

	
}

CDecimalRangeCtrl::~CDecimalRangeCtrl()
{
DestroyControl(m_pFrom);
DestroyControl(m_pSeparator);
DestroyControl(m_pTo);

}


BEGIN_MESSAGE_MAP(CDecimalRangeCtrl, CStatic)
	
		ON_WM_SETFOCUS()
		ON_WM_KILLFOCUS()
		ON_WM_ENABLE()
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecimalRangeCtrl message handlers
BOOL CDecimalRangeCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bOK = CStatic::Create(L"", WS_CHILD, rect, pParentWnd, nID);
	InitControls();
	PosControls();

	return bOK;
}



BOOL CDecimalRangeCtrl::InitControls()
{

		CWnd* pParent = GetParent();
  if (pParent != NULL)
    SetFont(pParent->GetFont());
    
   m_pFrom = new CDecimalEdit(m_precision);
    if (m_pFrom == NULL || !m_pFrom->CreateEx(WS_EX_CLIENTEDGE,L"EDIT",L"",WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, ID_FROM))
      return FALSE;
	
	//m_pFrom->ModifyStyleEx(0, WS_EX_WINDOWEDGE);
	m_pFrom->SetFont(GetFont());
	m_pFrom->EnableWindow(TRUE);
	m_pFrom->ShowWindow(SW_SHOWNOACTIVATE);
    
 // m_pDate1->SendMessage(WM_KILLFOCUS,0,0);


  m_pSeparator = new CStatic;
  if (m_pSeparator == NULL || !m_pSeparator->Create(L"-" , WS_CHILD | WS_VISIBLE | SS_NOPREFIX | SS_CENTER, CRect(0, 0, 0, 0), this, ID_LABEL))
      return FALSE;
  
  m_pSeparator->SetFont(GetFont());
  m_pSeparator->EnableWindow(TRUE);

  m_pSeparator->ShowWindow(SW_SHOWNOACTIVATE);
  
 m_pTo = new CDecimalEdit(m_precision);
    if (m_pTo == NULL || !m_pTo->CreateEx(WS_EX_CLIENTEDGE, L"EDIT", L"",WS_CHILD | WS_TABSTOP | WS_VISIBLE  | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, ID_TO))
      return FALSE;
  
	m_pTo->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_pTo->SetFont(GetFont());
	m_pTo->EnableWindow(TRUE);
	m_pTo->ShowWindow(SW_SHOWNOACTIVATE);
   // m_pDate2->SendMessage(WM_KILLFOCUS,0,0);

  return TRUE;
}

void CDecimalRangeCtrl::PosControls(void)
{
	const int H = 24;
	const int GAP = 3;
	const int W = 100;

  CRect rc;
  GetClientRect(rc);
  int cx = rc.Width();
  int cy = rc.Height();
  int posx,posy;
  CDC * pDC;
  CSize size;

  int separatorW = cx - (GAP*2 + 2*W);

  posx = 0;
  posy = rc.Width()/2;

  m_pFrom->MoveWindow(posx,(cy-H)/2,W,H);
  posx += W + GAP;  

  pDC = m_pSeparator->GetDC();
  size = pDC->GetTextExtent(L"-");

  posy = (cy - size.cy)/2;
  m_pSeparator->MoveWindow(posx,posy, separatorW,size.cy);
  m_pSeparator->ReleaseDC(pDC);
  posx += separatorW +GAP;

  m_pTo->MoveWindow(posx,(cy-H)/2,W,H);


}


DecimalRange CDecimalRangeCtrl::GetRange() const
{
	NullDecimal low,hi;
	
	low = m_pFrom->GetDecimal();
	
	hi = m_pTo->GetDecimal();
	
	return DecimalRange(low,hi);
}


void CDecimalRangeCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	SetWindowText(L"");
	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	InitControls();
	PosControls();
}


void CDecimalRangeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
}

void CDecimalRangeCtrl::OnKillFocus(CWnd* pNewWnd)
{
  CStatic::OnKillFocus(pNewWnd);
  m_pFrom->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
  m_pTo->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
}

void CDecimalRangeCtrl::OnEnable(BOOL bEnable) 
{
  CStatic::OnEnable(bEnable);


m_pFrom->EnableWindow(bEnable);
m_pSeparator->EnableWindow(bEnable);
m_pTo->EnableWindow(bEnable);



 SendMessage(WM_SETREDRAW, TRUE, 0);
 RedrawWindow( NULL, NULL,          RDW_INVALIDATE | RDW_ALLCHILDREN);



}