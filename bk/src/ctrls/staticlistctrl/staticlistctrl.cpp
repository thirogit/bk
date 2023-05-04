#include "stdafx.h"
#include "StaticListCtrl.h"
#include <boost\foreach.hpp>
#include "..\arrays\DeepDelete.h"

#define GAP 3
#define ROWH 18
#define HEADERH ROWH
#define SCROLLBARWIDTH 18
#define GRID_COLOR RGB(212,208,200)


BEGIN_MESSAGE_MAP(CStaticListCtrl::CHeader, CStatic)

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()


	

void CStaticListCtrl::CHeader::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	CStatic::Create(L"header", WS_VISIBLE | WS_CHILD | WS_TABSTOP | SS_NOTIFY, rect, pParentWnd, nID);
}

void CStaticListCtrl::CHeader::OnPaint()
{
	const UINT style = DFCS_BUTTONPUSH | DFCS_FLAT;
	const UINT uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;

	CPaintDC dc(this);

	CRect cr;
	GetClientRect(cr);

	dc.SaveDC();

	
	int x = 0;
	BOOST_FOREACH(CColumn column, m_parent->m_columns)
	{
		CRect cellRect(x, 0, x + column.width, ROWH);

		dc.DrawFrameControl(cellRect, DFC_BUTTON, style);
				
		dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));		
		dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
				
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(column.text.c_str(), column.text.length(), &cellRect, uFormat);

		dc.SelectStockObject(SYSTEM_FONT);
		
		x += column.width;
	}

	if (x <= cr.Width())
	{
		CRect remainingHeader(x, 0, cr.Width(), ROWH);
		dc.DrawFrameControl(remainingHeader, DFC_BUTTON, style);

	}

}
BOOL CStaticListCtrl::CHeader::OnEraseBkgnd(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);
	pDC->FillRect(&cr,  &m_parent->m_BkgBrush);

	return TRUE;
}
//#################################################################################################

BEGIN_MESSAGE_MAP(CStaticListCtrl::CBody, CStatic)
		
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
		
END_MESSAGE_MAP()

void CStaticListCtrl::CBody::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	CStatic::Create(L"body", WS_VISIBLE | WS_CHILD | WS_TABSTOP | SS_NOTIFY, rect, pParentWnd, nID);
}

BOOL CStaticListCtrl::CBody::OnEraseBkgnd(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);
	pDC->FillRect(&cr, &m_parent->m_BkgBrush);

	return TRUE;
}

void CStaticListCtrl::CBody::OnPaint()
{
	

	CPaintDC dc(this); 
}


BOOL CStaticListCtrl::CBody::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_parent->OnMouseWheel(nFlags, zDelta, pt);
	return FALSE;
}

//#################################################################################################

BEGIN_MESSAGE_MAP(CStaticListCtrl::CRow, CStatic)

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CStaticListCtrl::CRow::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}

void CStaticListCtrl::CRow::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	m_bLBtnDown = TRUE;
	m_parent->Select(this);
}

void CStaticListCtrl::CRow::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bLBtnDown)
	{
		
		m_bLBtnDown = FALSE;
	}

}

void CStaticListCtrl::CRow::OnPaint()
{	
	CPaintDC dc(this); 

	CRect cr;
	GetClientRect(cr);

	dc.SaveDC();
	
	
	dc.SelectObject(&m_GridPen);

	int x = 0;
	
	dc.MoveTo(x,0);
	dc.LineTo(cr.Width(),0);

	dc.MoveTo(x, ROWH);
	dc.LineTo(cr.Width(), ROWH);


	BOOST_FOREACH(CColumn column, m_parent->m_columns)
	{
		CRect cellRect(x, 0, x+column.width, ROWH);
		
		COLORREF clrCellBkb = RGB(255, 255, 255);
		std::wstring sItemText = m_row->CellValue(column.id);
		Color cellBkg = m_row->CellColor(column.id);
		if (!cellBkg.IsNull())
		{
			clrCellBkb = cellBkg;
		}

		if (m_parent->IsSelected(this))
		{
			dc.FillSolidRect(&cellRect, GetSysColor(COLOR_HIGHLIGHT));
			dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}

		dc.MoveTo(x, 0);
		dc.LineTo(x, ROWH);


		/*if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			dc.FillSolidRect(&cellRect, GetSysColor(COLOR_HIGHLIGHT));
			
		}
		else
		{*/
			
			//dc.FillSolidRect(&cellRect, clrCellBkb);
			
		//}
			//dc.Rectangle(&cellRect);
		dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));


		UINT uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;
			
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(sItemText.c_str(), sItemText.length(), &cellRect, uFormat);

		dc.SelectStockObject(SYSTEM_FONT);	

		x += column.width;
	}

	if (x <= cr.Width())
	{
		dc.MoveTo(x, 0);
		dc.LineTo(x, ROWH);
	}

	dc.RestoreDC(-1);
}



BOOL CStaticListCtrl::CRow::OnEraseBkgnd(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);
	pDC->FillRect(&cr, &m_parent->m_BkgBrush);

	return TRUE;
}


void CStaticListCtrl::CRow::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_bLBtnDown = FALSE;
	m_GridPen.CreatePen(PS_SOLID, 1, GRID_COLOR);
	
	CStatic::Create(L"", WS_VISIBLE | WS_CHILD | WS_TABSTOP | SS_NOTIFY, rect, pParentWnd, nID);
}

BOOL CStaticListCtrl::CRow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_parent->OnMouseWheel(nFlags, zDelta, pt);
	return FALSE;
}

//#################################################################################################

BEGIN_MESSAGE_MAP(CStaticListCtrl, COwnClassWnd)

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()

END_MESSAGE_MAP()



CStaticListCtrl::CStaticListCtrl() : COwnClassWnd(L"staticlist")
{
	m_BkgBrush.CreateSolidBrush(RGB(255, 255, 255));
	m_BkgDebugBrush.CreateSolidBrush(RGB(255, 0, 0));
	m_idSequence = 100;
	m_rowSelected = NULL;
}

void CStaticListCtrl::CreateHeader()
{
	CRect rect;
	GetClientRect(&rect);
	m_header.m_parent = this;
	m_header.Create(CRect(0, 0, rect.Width() - SCROLLBARWIDTH,ROWH ),this,1);
}

void CStaticListCtrl::CreateBody()
{
	CRect rect;
	GetClientRect(&rect);
	m_body.m_parent = this;
	m_body.Create(CRect(0, ROWH, rect.Width()- SCROLLBARWIDTH, rect.Height()), this, 2);
}

void CStaticListCtrl::CreateScrollBars()
{
	CRect rect;
	GetClientRect(&rect);
	m_vScrollBar.Create(SBS_VERT | WS_CHILD | WS_VISIBLE,CRect(rect.right- SCROLLBARWIDTH,0,rect.right,rect.Height()) , this, 3);
	m_scrollYPos = 1;
	m_scrollY = 0;
	
}

void CStaticListCtrl::UpdateScrollBars(int height)
{
	CRect rect;
	m_body.GetClientRect(&rect);


	int maxScrollDelta = rect.Height() - height;

	if ((height > rect.Height()) && (maxScrollDelta > m_scrollY))
	{
		int cy = -(m_scrollY - maxScrollDelta);
		m_scrollY += cy;
		m_body.ScrollWindow(0, cy, NULL, &rect);
		m_scrollYPos -= cy;
	}

	m_maxScrollY = height;
	
	
	SCROLLINFO si = {};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nPos = m_scrollYPos;
	si.nMin = 1;
	si.nMax = height;
	si.nPage = rect.Height();

	m_vScrollBar.SetScrollInfo(&si, TRUE);

	
}

BOOL CStaticListCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{	
	 COwnClassWnd::CreateEx(WS_EX_CONTROLPARENT,L"", dwStyle, rect, pParentWnd, nID);
	
	 CreateHeader();
	 CreateBody();
	 CreateScrollBars();
	 return TRUE;
}



void CStaticListCtrl::OnSize(UINT nType, int cx, int cy)
{
	COwnClassWnd::OnSize(nType, cx, cy);
}

int CStaticListCtrl::GetScrollPos(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si = {};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;
	m_vScrollBar.GetScrollInfo(&si);

	const int minPos = si.nMin;
	const int maxPos = si.nMax - (si.nPage - 1);

	int result = -1;

	switch (nSBCode)
	{
	case SB_LINEUP /*SB_LINELEFT*/:
		result = max(si.nPos - ROWH, minPos);
		break;

	case SB_LINEDOWN /*SB_LINERIGHT*/:
		result = min(si.nPos + ROWH, maxPos);
		break;

	case SB_PAGEUP /*SB_PAGELEFT*/:
		result = max(si.nPos - (int)si.nPage, minPos);
		break;

	case SB_PAGEDOWN /*SB_PAGERIGHT*/:
		result = min(si.nPos + (int)si.nPage, maxPos);
		break;

	case SB_THUMBPOSITION:
		// do nothing
		break;

	case SB_THUMBTRACK:
		result = si.nTrackPos;
		break;

	case SB_TOP /*SB_LEFT*/:
		result = minPos;
		break;

	case SB_BOTTOM /*SB_RIGHT*/:
		result = maxPos;
		break;

	case SB_ENDSCROLL:
		// do nothing
		break;
	}

	return result;
}

void CStaticListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	

	int pos = GetScrollPos(nSBCode, nPos, pScrollBar);
	if (pos == -1)
		return;

	pScrollBar->SetScrollPos(pos, TRUE);
	CRect bodyRect;
	m_body.GetClientRect(&bodyRect);
	
	int cy = 0;

	
	cy = m_scrollYPos - pos;
	m_scrollYPos = pos;

	if (cy)
	{
		m_scrollY += cy;
		m_body.ScrollWindow(0, cy, NULL, &bodyRect);
		//m_header.Invalidate(FALSE);
	}

}

void CStaticListCtrl::OnDestroy()
{
	COwnClassWnd::OnDestroy();

	/*BOOST_FOREACH(Control* control, m_controls)
	{
		if (control->header)
			delete control->header;
	}

	DeepDelete(m_controls);*/
}

int CStaticListCtrl::GetWindowH(CWnd* pWnd)
{
	int H = 0;
	if (pWnd)
	{
		CRect rect;
		pWnd->GetClientRect(&rect);
		H = rect.Height();
	}

	return H;
}

int CStaticListCtrl::ComputeHeight()
{
	int H = 0;
	BOOST_FOREACH(CRow* row, m_rows)
	{		
		H += GetWindowH(row);
	}
	return H;
}

int CStaticListCtrl::ComputeWidth()
{
	int w = 0;

	BOOST_FOREACH(CColumn column, m_columns)
	{
		w += column.width;
	}
	return w;
}

void CStaticListCtrl::InsertColumn(const std::wstring& sColumnText,uint32_t id,uint32_t width)
{
	CColumn column;
	column.id = id;
	column.text = sColumnText;
	column.width = width;
	
	m_columns.push_back(column);
}

void CStaticListCtrl::AddRow(ListRow* row)
{
	CRect client;
	m_body.GetClientRect(&client);

	int h = ComputeHeight();
	int w = max(ComputeWidth(), client.Width());


	CRow* rowWnd = new CRow;
	rowWnd->m_parent = this;
	rowWnd->m_row = row;
	rowWnd->m_bSelected = false;

	CRect rect(0,h,w,h+ROWH);
	rowWnd->Create(rect, &m_body, m_idSequence++);
		
	UpdateScrollBars(h+ROWH);

	//TRACE(L"updating sb height = %d\n", h);
		
	m_rows.push_back(rowWnd);
}

void CStaticListCtrl::PreSubclassWindow()
{
	COwnClassWnd::PreSubclassWindow();
	/*ModifyStyleEx(GetExStyle(), WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP | SS_NOTIFY);
	CreateHeader();
	CreateBody();
	CreateScrollBars();*/
}

void CStaticListCtrl::OnPaint()
{
	//CWnd::OnPaint();

	CPaintDC dc(this); // device context for painting

	dc.SaveDC();

	
	dc.RestoreDC(-1);
}

BOOL CStaticListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);
	pDC->FillRect(&cr, &m_BkgBrush);

	return TRUE;
}


void CStaticListCtrl::Select(CRow* row)
{
	if (m_rowSelected != NULL)
	{
		if (m_rowSelected == row)
			return;

		m_rowSelected->m_bSelected = false;
		m_rowSelected->Invalidate();
	}

	row->m_bSelected = !row->m_bSelected;
	row->Invalidate(FALSE);
	m_rowSelected = row;
}


bool CStaticListCtrl::IsSelected(CRow* row)
{
	return row->m_bSelected;
}

CPoint CStaticListCtrl::GetRowPosition(CRow* row)
{
	CRect rowRect;
	row->GetWindowRect(&rowRect);
	m_body.ScreenToClient(&rowRect);

	return CPoint(rowRect.left, rowRect.top);
}

void CStaticListCtrl::RepositionRowsBelow(CRow* row,int deltaY)
{
	const boost::multi_index::index<RowsIndex, index_tag_id>::type& indexById = m_rows.get<index_tag_id>();

	CRect rowRect;
	row->GetWindowRect(&rowRect);
	m_body.ScreenToClient(&rowRect);

	auto rowIt = indexById.find(row->GetDlgCtrlID());
	rowIt++;
	

	while (rowIt != indexById.end())
	{
		CRow* nextRow = *rowIt;
		CPoint rowPosition = GetRowPosition(nextRow);
		nextRow->SetWindowPos(NULL, rowPosition.x, rowPosition.y+deltaY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		rowIt++;
	}	
}

BOOL CStaticListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT nSBCode = (zDelta / WHEEL_DELTA) > 0 ? SB_LINEUP : SB_LINEDOWN;

	OnVScroll(nSBCode, m_vScrollBar.GetScrollPos(), &m_vScrollBar);

	return TRUE;
}