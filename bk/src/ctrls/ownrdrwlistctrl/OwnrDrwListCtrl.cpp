#include "stdafx.h"
#include "OwnrDrwListCtrl.h"

#define CELL_MARGIN 5

BEGIN_MESSAGE_MAP(COwnrDrwListCtrl,CContentListCtrl)
	
	ON_WM_MEASUREITEM_REFLECT()
	
END_MESSAGE_MAP()

COwnrDrwListCtrl::COwnrDrwListCtrl() : m_ItemHeight(0)
{
}

void COwnrDrwListCtrl::SelectRow(int row)
{
	if(row >= 0 && row < GetItemCount())
		SetItemState(row,LVIS_SELECTED,LVIS_SELECTED);
}

void COwnrDrwListCtrl::UnselectRow(int row)
{
	if(row >= 0 && row < GetItemCount())
		SetItemState(row,0,LVIS_SELECTED);
}

void COwnrDrwListCtrl::SelectAll()
{
	for(int row = 0,count = GetItemCount();row < count;row++)
	{
		SetItemState(row,LVIS_SELECTED,LVIS_SELECTED);
	}
}

void COwnrDrwListCtrl::UnselectAll()
{
	for(int row = 0,count = GetItemCount();row < count;row++)
	{
		SetItemState(row,0,LVIS_SELECTED);
	}
}

void COwnrDrwListCtrl::SetItemHeight(int itemH)
{
	m_ItemHeight = itemH;

	if (::IsWindow(m_hWnd))
	{
		CRect rc;
		GetWindowRect(&rc);

		WINDOWPOS wp;
		wp.hwnd = m_hWnd;
		wp.cx = rc.Width();
		wp.cy = rc.Height();
		wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
		SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
	}

}

void COwnrDrwListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	if(m_ItemHeight > 0)
	{
		lpMeasureItemStruct->itemHeight = m_ItemHeight;    
	}
}

void COwnrDrwListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	LV_COLUMN lvc;
	::ZeroMemory(&lvc, sizeof(lvc));	
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	
	int nIndex;

	CRect cellRect(lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.bottom);

	
	for ( int nCol=0,nCount = GetColumnCount(); nCol < nCount; nCol++)
	{
		nIndex = m_header.OrderToIndex(nCol);
		GetColumn(nIndex, &lvc);

		cellRect.left = cellRect.right;
		cellRect.right += lvc.cx;	

		if(cellRect.right  < 0)
			continue;

		std::wstring sItemText = CLayoutListCtrl::GetCellText(lpDrawItemStruct->itemID, nCol);
		Color cellBkb = CLayoutListCtrl::GetCellColor(lpDrawItemStruct->itemID, nCol);

		CDC* pDC;
		pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&cellRect, GetSysColor(COLOR_HIGHLIGHT)) ; 
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			pDC->FillSolidRect(&cellRect, (COLORREF)cellBkb) ;
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT)) ; 
		}
		
		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		
		UINT uFormat    =  DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX  | DT_END_ELLIPSIS ;

		//CRect textRect;
		//pDC->DrawText(lpBuffer, strlen(lpBuffer), &textRect, DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT) ;

		pDC->DrawText(sItemText.c_str(), sItemText.length(), &cellRect, uFormat) ;

		pDC->SelectStockObject(SYSTEM_FONT) ;
	}
}

