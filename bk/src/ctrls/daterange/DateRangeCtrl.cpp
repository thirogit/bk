#include "stdafx.h"
#include "DateRangeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DestroyControl(ctrl) \
	if (ctrl != NULL) { ctrl->DestroyWindow(); delete ctrl; ctrl = NULL; }


enum { ID_DATEEDITFROM = 3333, ID_SEPLABEL, ID_DATEEDITTO };


CDateRangeCtrl::CDateRangeCtrl() : CStatic(),m_pDateFrom(NULL), m_pDateTo(NULL), m_pSeparator(NULL)
{

	
}

CDateRangeCtrl::~CDateRangeCtrl()
{
DestroyControl(m_pDateFrom);
DestroyControl(m_pSeparator);
DestroyControl(m_pDateTo);

}


BEGIN_MESSAGE_MAP(CDateRangeCtrl, CStatic)
	
		ON_WM_SETFOCUS()
		ON_WM_KILLFOCUS()
		ON_WM_ENABLE()
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateRangeCtrl message handlers
BOOL CDateRangeCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bOK = CStatic::Create(L"", WS_CHILD, rect, pParentWnd, nID);
	InitControls();
	PosControls();

	return bOK;
}



BOOL CDateRangeCtrl::InitControls()
{

		CWnd* pParent = GetParent();
  if (pParent != NULL)
    SetFont(pParent->GetFont());
    
   m_pDateFrom = new CDateTimeCtrl;
    if (m_pDateFrom == NULL || !m_pDateFrom->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN, CRect(0, 0, 0, 0), this, ID_DATEEDITFROM))
      return FALSE;
  
	m_pDateFrom->SetFont(GetFont());
	m_pDateFrom->EnableWindow(TRUE);
	m_pDateFrom->ShowWindow(SW_SHOWNOACTIVATE);
    
 // m_pDate1->SendMessage(WM_KILLFOCUS,0,0);


  m_pSeparator = new CStatic;
  if (m_pSeparator == NULL || !m_pSeparator->Create(L"-" , WS_CHILD | WS_VISIBLE | SS_NOPREFIX | SS_CENTER, CRect(0, 0, 0, 0), this, ID_SEPLABEL))
      return FALSE;
  
  m_pSeparator->SetFont(GetFont());
  m_pSeparator->EnableWindow(TRUE);

  m_pSeparator->ShowWindow(SW_SHOWNOACTIVATE);
  
 m_pDateTo = new CDateTimeCtrl;
    if (m_pDateTo == NULL || !m_pDateTo->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN, CRect(0, 0, 0, 0), this, ID_DATEEDITTO))
      return FALSE;
  
	m_pDateTo->SetFont(GetFont());
	m_pDateTo->EnableWindow(TRUE);
	m_pDateTo->ShowWindow(SW_SHOWNOACTIVATE);
   // m_pDate2->SendMessage(WM_KILLFOCUS,0,0);

  return TRUE;
}

void CDateRangeCtrl::PosControls(void)
{

	const int GAP = 3;
	const int DATEW = 100;
	const int DATEH = 25;

  CRect rc;
  GetClientRect(rc);
  int cx = rc.Width();
  int cy = rc.Height();
  int posx,posy;
  CDC * pDC;
  CSize size;

  int separatorW = cx - (GAP*2 + 2*DATEW);

  posx = 0;
  posy = rc.Width()/2;

  m_pDateFrom->MoveWindow(posx,(cy- DATEH)/2,DATEW, DATEH);
  posx += DATEW + GAP;  

  pDC = m_pSeparator->GetDC();
  size = pDC->GetTextExtent(L"-");

  posy = (cy - size.cy)/2;
  m_pSeparator->MoveWindow(posx,posy, separatorW,size.cy);
  m_pSeparator->ReleaseDC(pDC);
  posx += separatorW +GAP;

  m_pDateTo->MoveWindow(posx,(cy- DATEH)/2,DATEW, DATEH);


}


DateRange CDateRangeCtrl::GetRange() const
{
	CTime time1, time2;
	DateTime t1,t2;
	m_pDateFrom->GetTime(time1);
	t1.SetYearMonthDay(time1.GetYear(),time1.GetMonth(),time1.GetDay());
	m_pDateTo->GetTime(time2);
	t2.SetYearMonthDay(time2.GetYear(),time2.GetMonth(),time2.GetDay());		
	
	return DateRange(t1,t2);
}

void CDateRangeCtrl::SetRange(const DateRange& range)
{
	DateTime start = range.GetStrDt();
	DateTime end = range.GetEndDt();

	CTime startCTime(start.GetYear(),start.GetMonth(),start.GetDay(),start.GetHour(),start.GetMinutes(),start.GetSeconds());
	CTime endCTime(end.GetYear(), end.GetMonth(), end.GetDay(), end.GetHour(), end.GetMinutes(), end.GetSeconds());

	m_pDateFrom->SetTime(&startCTime);
	m_pDateTo->SetTime(&endCTime);
}


void CDateRangeCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	InitControls();
	PosControls();
}


void CDateRangeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
}

void CDateRangeCtrl::OnKillFocus(CWnd* pNewWnd)
{
  CStatic::OnKillFocus(pNewWnd);
  m_pDateFrom->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
  m_pDateTo->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
}

void CDateRangeCtrl::OnEnable(BOOL bEnable) 
{
  CStatic::OnEnable(bEnable);


m_pDateFrom->EnableWindow(bEnable);
m_pSeparator->EnableWindow(bEnable);
m_pDateTo->EnableWindow(bEnable);



 SendMessage(WM_SETREDRAW, TRUE, 0);
 RedrawWindow( NULL, NULL,          RDW_INVALIDATE | RDW_ALLCHILDREN);



}