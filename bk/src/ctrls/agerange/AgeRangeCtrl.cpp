#include "stdafx.h"
#include "AgeRangeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DestroyControl(ctrl) \
	if (ctrl != NULL) { ctrl->DestroyWindow(); delete ctrl; ctrl = NULL; }


enum { ID_FROM = 3333, ID_LABEL, ID_TO ,ID_UNIT};


CAgeRangeCtrl::CAgeRangeCtrl() : CStatic(),m_pFrom(NULL), m_pTo(NULL), m_pSeparator(NULL), m_pUnit(NULL)
{

	
}

CAgeRangeCtrl::~CAgeRangeCtrl()
{
DestroyControl(m_pFrom);
DestroyControl(m_pSeparator);
DestroyControl(m_pTo);
DestroyControl(m_pUnit);

}


BEGIN_MESSAGE_MAP(CAgeRangeCtrl, CStatic)

	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ENABLE()
	
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgeRangeCtrl message handlers
BOOL CAgeRangeCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bOK = CStatic::Create(L"", WS_CHILD, rect, pParentWnd, nID);
	InitControls();
	PosControls();

	return bOK;
}



BOOL CAgeRangeCtrl::InitControls()
{

		CWnd* pParent = GetParent();
  if (pParent != NULL)
    SetFont(pParent->GetFont());
    
   m_pFrom = new CIntegerEdit;
    if (m_pFrom == NULL || !m_pFrom->CreateEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, ID_FROM))
      return FALSE;
  
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
  
 m_pTo = new CIntegerEdit;
    if (m_pTo == NULL || !m_pTo->CreateEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, ID_TO))
      return FALSE;
  
	m_pTo->SetFont(GetFont());
	m_pTo->EnableWindow(TRUE);
	m_pTo->ShowWindow(SW_SHOWNOACTIVATE);
   // m_pDate2->SendMessage(WM_KILLFOCUS,0,0);


	m_pUnit = new CDayWeekMonthBtn;
	if (m_pUnit == NULL || !m_pUnit->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, ID_UNIT))
		return FALSE;

	m_pUnit->SetFont(GetFont());
	m_pUnit->EnableWindow(TRUE);
	m_pUnit->ShowWindow(SW_SHOWNOACTIVATE);

  return TRUE;
}

void CAgeRangeCtrl::PosControls(void)
{
	const int H = 24;
	const int GAP = 3;
	const int INTW = 70;
	const int UNITW = 48;

  CRect rc;
  GetClientRect(rc);
  int cx = rc.Width();
  int cy = rc.Height();
  int posx,posy;
  CDC * pDC;
  CSize size;

  int separatorW = cx - (GAP*2 + 2* INTW + GAP + UNITW);

  posx = 0;
  posy = rc.Width()/2;

  m_pFrom->MoveWindow(posx,(cy-H)/2, INTW,H);
  posx += INTW + GAP;

  pDC = m_pSeparator->GetDC();
  size = pDC->GetTextExtent(L"-");

  posy = (cy - size.cy)/2;
  m_pSeparator->MoveWindow(posx,posy, separatorW,size.cy);
  m_pSeparator->ReleaseDC(pDC);
  posx += separatorW +GAP;

  m_pTo->MoveWindow(posx,(cy-H)/2, INTW,H);
  posx += INTW + GAP;

  m_pUnit->MoveWindow(posx, (cy - H) / 2, UNITW, H);

}


IntRange CAgeRangeCtrl::GetRange() const
{
	NullInt low,hi;
	
	low = m_pFrom->GetInteger();
	
	hi = m_pTo->GetInteger();
	
	return IntRange(low,hi);
}


CalendarUnit CAgeRangeCtrl::GetUnit() const
{
	return m_pUnit->GetUnit();
}

void CAgeRangeCtrl::PreSubclassWindow()
{

	CStatic::PreSubclassWindow();

	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	InitControls();
	PosControls();
}


void CAgeRangeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
}

void CAgeRangeCtrl::OnKillFocus(CWnd* pNewWnd)
{
  CStatic::OnKillFocus(pNewWnd);
  m_pFrom->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
  m_pTo->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
}

void CAgeRangeCtrl::OnEnable(BOOL bEnable) 
{
  CStatic::OnEnable(bEnable);


m_pFrom->EnableWindow(bEnable);
m_pSeparator->EnableWindow(bEnable);
m_pTo->EnableWindow(bEnable);



 SendMessage(WM_SETREDRAW, TRUE, 0);
 RedrawWindow( NULL, NULL,          RDW_INVALIDATE | RDW_ALLCHILDREN);



}