#include "stdafx.h"
#include "LayoutListCtrl.h"
#include <algorithm>
#include "../../resource.h"

ListSelection::ListSelection()
{
}

ListSelection::ListSelection(const ListSelection& src)
{
	operator=(src);
}

ListSelection& ListSelection::operator=(const ListSelection& right)
{
	m_rows.clear();
	std::for_each(right.m_rows.begin(),
				  right.m_rows.end(),
				  [this](int row)
				  {
					  m_rows.push_back(row);
				  });	
	return *this;
}

size_t ListSelection::GetCount() const
{
	return m_rows.size();
}

void ListSelection::AddRow(int row)
{
	m_rows.push_back(row);
}

SelectionRange ListSelection::GetRange()
{
	return SelectionRange(m_rows);
}


	
//-------------------------------------------------------------------------------------------------

CellDescriptor::CellDescriptor(int row,int col,const CRect& rect)
{
	m_row = row;
	m_col = col;
	m_rect = rect;
}

int CellDescriptor::GetRow() const
{
	return m_row;
}

int CellDescriptor::GetCol() const
{
	return m_col;
}

const CRect& CellDescriptor::GetRect() const
{
	return m_rect;
}

//-------------------------------------------------------------------------------------------------

#define IDM_HEADERMENU_MIN 1001
#define IDM_HEADERMENU_MAX (IDM_HEADERMENU_MIN+1000)

BEGIN_MESSAGE_MAP(CLayoutListCtrl,CListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO,GetDispInfo)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw )	
	ON_NOTIFY_REFLECT(NM_DBLCLK , OnDblClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)

	

	ON_COMMAND_RANGE(IDM_HEADERMENU_MIN,IDM_HEADERMENU_MAX,OnHeaderMenu)
	
END_MESSAGE_MAP()



CLayoutListCtrl::CLayoutListCtrl() : m_pColumnLayout(NULL)
{
}

CLayoutListCtrl::CLayoutListCtrl(const ListHeaderDescriptor& headerDescriptor) : m_pColumnLayout(NULL),m_HeaderDesc(headerDescriptor)
{
}

void CLayoutListCtrl::SetHeaderDescriptor(const ListHeaderDescriptor& headerDescriptor)
{
	m_HeaderDesc = headerDescriptor;	
	
	if(::IsWindow(m_hWnd))
	{
		BuildHeader();
	}
}

void CLayoutListCtrl::SetHeaderLayout(ListHeaderLayout* pHeaderLayout)
{
	m_pColumnLayout = pHeaderLayout;
	if (::IsWindow(m_hWnd))
	{
		BuildHeader();
	}
}

CLayoutListCtrl::~CLayoutListCtrl()
{
	
}

void CLayoutListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();	
}

void CLayoutListCtrl::OnHeaderMenu(UINT nId)
{
	MENUITEMINFO menuItem;
	memset(&menuItem,0,sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
	menuItem.fMask = MIIM_DATA;
	m_headerMenu.GetMenuItemInfo(nId,&menuItem,FALSE);

	if(m_pColumnLayout->ContainsColumnWithId(menuItem.dwItemData))
	{
		RemoveColumn(menuItem.dwItemData);
	}
	else
	{
		AppendColumn(menuItem.dwItemData);
	}

}

void CLayoutListCtrl::AppendColumn(uint32_t colId)
{
	const HeaderColumn* pColumn = m_HeaderDesc.GetColumnWithId(colId);
	CDC *dc = GetDC();
	m_pColumnLayout->AddColumnLayout(pColumn->GetColId(),dc->GetTextExtent(pColumn->GetColText().c_str()).cx);
	ReleaseDC(dc);
	BuildHeader();
}

void CLayoutListCtrl::RemoveColumn(uint32_t colId)
{
	m_pColumnLayout->RemoveColumnLayout(colId);
	BuildHeader();
}

void CLayoutListCtrl::BuildHeaderMenu()
{
	m_headerMenu.DestroyMenu();

	m_headerMenu.CreatePopupMenu();

	MENUITEMINFO menuItem;
	memset(&menuItem,0,sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
				
	PtrEnumerator<HeaderColumn> colEnum = m_HeaderDesc.EnumColumns();
	HeaderColumn* pCol;
	std::wstring itemText;
	UINT nId = IDM_HEADERMENU_MIN;
	while(colEnum.hasNext())
	{
		memset(&menuItem,0,sizeof(MENUITEMINFO));
		menuItem.cbSize = sizeof(MENUITEMINFO);		
		pCol = *colEnum;

		itemText = pCol->GetColText();

		menuItem.fMask =  MIIM_STRING | MIIM_ID | MIIM_DATA | MIIM_STATE;
		menuItem.dwTypeData = const_cast<wchar_t*>(itemText.c_str());
		menuItem.cch = itemText.length();
		menuItem.fState = m_pColumnLayout->ContainsColumnWithId(pCol->GetColId()) ? MFS_CHECKED : MFS_UNCHECKED;

		menuItem.wID = nId;

		menuItem.dwItemData = (ULONG_PTR)pCol->GetColId();		

		m_headerMenu.InsertMenuItem(m_headerMenu.GetMenuItemCount(),&menuItem,TRUE);

		nId++;
		colEnum.advance();
	}	
}

void CLayoutListCtrl::BuildHeader()
{		
	CDC *dc = GetDC();
	int i,s;
	LV_COLUMN lvcolumn;

	//lvcolumn.mask = LVCF_TEXT | LVCF_WIDTH;
	//lvcolumn.pszText = "";
	//lvcolumn.cx = 1;
	//InsertColumn(0, &lvcolumn);		
	
	if(GetHeaderCtrl())
	{
		for(i = 0,s = GetColumnCount();i < s;i++) 
			DeleteColumn(0);
	}	

	const HeaderColumn* pColumn;
	if(m_pColumnLayout == NULL)
	{
		m_pColumnLayout = &m_LocalLayout;
		m_pColumnLayout->Clear();
	}

	if(m_pColumnLayout->IsEmpty())
	{
		PtrEnumerator<HeaderColumn> headerIt = m_HeaderDesc.EnumColumns();
		while(headerIt.hasNext())
		{
			pColumn  = *headerIt;
			m_pColumnLayout->AddColumnLayout(pColumn->GetColId(),dc->GetTextExtent(pColumn->GetColText().c_str()).cx);
			headerIt.advance();
		}
	}
		
	wchar_t szColText[80];
	
	for(int col = 0;col < m_pColumnLayout->GetColumnCount();col++)
	{
		ColumnLayout* colLayout = m_pColumnLayout->GetColumnAt(col);
		pColumn = m_HeaderDesc.GetColumnWithId(colLayout->GetColId());
		wcscpy_s(szColText,pColumn->GetColText().c_str());
		memset(&lvcolumn, 0, sizeof(lvcolumn));		
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = szColText;
		lvcolumn.iSubItem = col;
		lvcolumn.cx = colLayout->GetColWidth();
		InsertColumn(col, &lvcolumn);		
	}

	ReleaseDC(dc);
	BuildHeaderMenu();

}


void CLayoutListCtrl::PostCreate()
{	
	EnableToolTips(TRUE);
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	
	BuildHeader();	

	m_sortArrows.Create(14,14,ILC_COLORDDB | ILC_MASK,2,1);
	CBitmap sortArrowsBmp;
	sortArrowsBmp.LoadBitmap(IDB_SORTARROWS);
	m_sortArrows.Add(&sortArrowsBmp,RGB(255,0,255));


	
	if(m_header.m_hWnd == NULL)
		m_header.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	m_header.SetImageList(&m_sortArrows);

	
}

void CLayoutListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	ASSERT(GetStyle() & LVS_OWNERDATA);
	
	m_header.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	if(m_header.m_hWnd != NULL)
	{
		PostCreate();	
	}
}
int CLayoutListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int createResult = CListCtrl::OnCreate(lpCreateStruct);
	if(createResult != 0) return createResult;

	ASSERT(GetStyle() & LVS_OWNERDATA);
	
	if(m_header.m_hWnd == NULL)
	{
		m_header.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
		PostCreate();	
	}
	return 0;
}

int CLayoutListCtrl::GetItemImage(int iItem,int Column)
{
	if(m_CellImageResolver)
		return m_CellImageResolver(iItem, GetColId(Column));

	return -1;
}

void CLayoutListCtrl::GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	if (pItem->iItem >= 0 && pItem->iSubItem >= 0)
	{
		if (pItem->mask & LVIF_TEXT)
		{
			std::wstring sCellText = GetCellText(pItem->iItem, GetColumnOrder(pItem->iSubItem));
			wcscpy_s(pItem->pszText, pItem->cchTextMax, sCellText.c_str());
		}

		//if (pItem->mask & LVIF_IMAGE)
		{
			pItem->iImage = GetItemImage(pItem->iItem, GetColumnOrder(pItem->iSubItem));
		}
	}
	*pResult = 0;
}

CRect CLayoutListCtrl::GetCellRect(int Item, int Column)
{
	if (!EnsureVisible (Item, TRUE)) 
    		return CRect();
	    
	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	if (Column >= nColumnCount)
		return CRect();                 

	// Get the column offset
	int Offset = 0;
	for (int iColumn = 0; iColumn < Column; iColumn++)
		Offset += GetColumnWidth (iColumn);

	CRect Rect;
	GetItemRect (Item, &Rect, LVIR_BOUNDS);

	// Now scroll if we need to expose the column
	CRect ClientRect;
	GetClientRect (&ClientRect);
	if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
	{
		CSize Size;
		if (Offset + Rect.left > 0)
			Size.cx = -(Offset - Rect.left);
		else
			Size.cx = Offset - Rect.left;
		Size.cy = 0;
		Scroll (Size);
		Rect.left -= Size.cx;
	}
		
	Rect.left += Offset/*+4*/ + 2;
	Rect.right = Rect.left + GetColumnWidth (Column) - 3;
	if (Rect.right > ClientRect.right)	Rect.right = ClientRect.right;
	
	return Rect;
}

BOOL CLayoutListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	OnColClick(GetColumnOrder(pnmlv->iSubItem));
	*pResult = 0;
	return TRUE;		// return FALSE to send message to parent also -
						// NOTE:  MSDN documentation is incorrect
}

int CLayoutListCtrl::GetColumnCount()
{
	return GetHeaderCtrl()->GetItemCount();
}

int CLayoutListCtrl::GetCurSel()
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = GetNextSelectedItem(pos);
	return nSelectedItem;
}

void CLayoutListCtrl::OnColClick(int nCol)
{
	SortByCol(GetColId(nCol));
	Invalidate(FALSE);
}

uint32_t CLayoutListCtrl::GetColId(int nCol)
{
	if(nCol >= 0 &&  nCol < m_pColumnLayout->GetColumnCount())
		return m_pColumnLayout->GetColumnAt(nCol)->GetColId();

	return (uint32_t)-1;
}

void CLayoutListCtrl::SortByCol(uint32_t columnId)
{
	DWORD ctrlKeyState = GetAsyncKeyState(VK_CONTROL);
	
	SortOrder colOrdering = SortOrder_Ascending;
	const SortAttribute* attribute = m_sortAttributes.GetAttribute(columnId);
	if(attribute != NULL)
		colOrdering = attribute->Order();

	colOrdering = (colOrdering == SortOrder_Ascending ? SortOrder_Descending : SortOrder_Ascending);
	
	SortAttribute newAttribute(columnId,colOrdering);

	if (ctrlKeyState & 0x8000)
	{
		//bool bRep = false;

		//if(!m_shiftPressLastSort)
		//{
		//	m_sortAttributes.Clear();
		//}

		const SortAttribute* attribute = m_sortAttributes.GetAttribute(columnId);
		
		if(attribute)
		{
			m_sortAttributes.SetAttribute(newAttribute);
		}
		else		
		{
			m_sortAttributes.AddAttribute(newAttribute);
		}
		//m_shiftPressLastSort = true;
	}
	else
	{
		m_sortAttributes.Clear();
		m_sortAttributes.AddAttribute(newAttribute);
		//m_shiftPressLastSort = false;
	}

	SortBy(m_sortAttributes);
	RepaintSortArrows();	
}

void CLayoutListCtrl::SortBy(const SortAttributes& attributes)
{

}

void CLayoutListCtrl::RepaintSortArrows()
{

	CHeaderCtrl *header = GetHeaderCtrl();

	uint32_t colId;
	SortOrder order;
	HDITEM hdItem;
	        
	for(int i = 0,count = header->GetItemCount();i < count;i++)
	{
		colId = GetColId(i);
		order = SortOrder_None;
		hdItem.mask = HDI_IMAGE | HDI_FORMAT;
		VERIFY( header->GetItem(i, &hdItem) );
		
		const SortAttribute* attribute = m_sortAttributes.GetAttribute(colId);
		if(attribute != NULL)
			order = attribute->Order();


		switch(order)
		{
		case SortOrder_Ascending:
			hdItem.iImage = 0;
			hdItem.fmt |= HDF_IMAGE;
			break;

		case SortOrder_Descending:
			hdItem.iImage = 1;
			hdItem.fmt |= HDF_IMAGE;
			break;

		case SortOrder_None:
			hdItem.fmt &= (~HDF_IMAGE);
			break;
		}
			
		header->SetItem(i,&hdItem);

	}
}


std::wstring CLayoutListCtrl::GetCellText(int nItem,int nCol)
{
	if(m_CellTextResolver)
	{		
		return m_CellTextResolver(nItem,GetColId(nCol));
	}
	return std::wstring();
}

void CLayoutListCtrl::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    *pResult = CDRF_DODEFAULT;
    
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		*pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
    {		
		pLVCD->clrTextBk = GetCellColor(pLVCD->nmcd.dwItemSpec,GetColumnOrder(pLVCD->iSubItem));		
		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
    }
}

Color CLayoutListCtrl::GetCellColor(int nItem,int nSubItem)
{
	if(m_CellColorResolver)
	{		
		Color color = m_CellColorResolver(nItem,GetColId(nSubItem));
		if(!color.IsNull())
			return color;
	}
	return (nItem & 1) ? Color(255, 255, 255) : Color(255, 255, 225);
}

void CLayoutListCtrl::Lock()
{
	SetRedraw(FALSE);
}
void CLayoutListCtrl::Unlock()
{
	SetRedraw(TRUE);
}

void CLayoutListCtrl::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (pNMIA && pNMIA->iItem >= 0)
	{
		
		if(m_CellDblClickHandler)
		{
			int row = pNMIA->iItem;
			int col = GetColumnOrder(pNMIA->iSubItem);
			m_CellDblClickHandler(row,GetColId(col),CellDescriptor(row,col,GetCellRect(row,col)));
		}
		else
		{
			if(m_RowDblClickHandler)
				m_RowDblClickHandler(pNMIA->iItem);
		}
	}
	*pResult = 0;
}

void CLayoutListCtrl::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA)
	{
		if(m_RightClickHandler)
		{
			int ncol = GetColumnOrder(pNMIA->iSubItem);
			m_RightClickHandler(pNMIA->iItem, GetColId(ncol), CellDescriptor(pNMIA->iItem, ncol, GetCellRect(pNMIA->iItem, ncol)));
		}		
	}
	*pResult = 0;
}

void CLayoutListCtrl::SetCellDblClickHandler(std::function<void (int,uint32_t,CellDescriptor& cell)>& handler)
{
	m_CellDblClickHandler = handler;
}

void CLayoutListCtrl::SetRowDblClickHandler(std::function<void (int)>& handler)
{
	m_RowDblClickHandler = handler;
}

void CLayoutListCtrl::SetCellTextResolver(std::function<std::wstring (int,uint32_t)>& resolver)
{
	m_CellTextResolver = resolver;
}

void CLayoutListCtrl::SetCellColorResolver(std::function<Color (int,uint32_t)>& resolver)
{
	m_CellColorResolver = resolver;
}

void CLayoutListCtrl::SetCellImageResolver(std::function<int (int,uint32_t)>& resolver)
{
	 m_CellImageResolver = resolver;
}

void CLayoutListCtrl::SetOnRightClick(std::function<void (int,uint32_t,CellDescriptor& cell)>& handler)
{
	m_RightClickHandler = handler;
}

void CLayoutListCtrl::OnColumnEndDrag(int iItem, int iOrder)
{
	m_pColumnLayout->MoveColumn(GetColumnOrder(iItem), iOrder);
}

void CLayoutListCtrl::OnColumnEndTrack(int iItem, int width)
{
	m_pColumnLayout->GetColumnAt(GetColumnOrder(iItem))->SetColWidth(width);
}

int CLayoutListCtrl::GetColumnOrder(int ColumIndex)
{
	if (ColumIndex >= 0 && ColumIndex < m_header.GetItemCount())
	{
		HDITEM item = { 0 };
		item.mask = HDI_ORDER;
		m_header.GetItem(ColumIndex, &item);
		return item.iOrder;
	}
	return -1;
}

BOOL CLayoutListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch(((NMHDR*)lParam)->code)
	{
	case NM_RCLICK:
		{
			POINT Point;
			GetCursorPos (&Point);
			ScreenToClient(&Point);
        
			HDHITTESTINFO HitTest;
       
			//Offset of right scrolling  
			HitTest.pt.x = Point.x+ GetScrollPos(SB_HORZ); //Offset of right scrolling
			HitTest.pt.y = Point.y;
        
			//Send the Hit Test Message
			int nCol = m_header.SendMessage(HDM_HITTEST,0,(LPARAM)&HitTest);
			OnColRClick(nCol,CPoint(Point));
			*pResult = 0;
			return TRUE;
		}
	case HDN_ENDDRAG:
		{
			NMHEADER* pNMHeader = (LPNMHEADER)lParam;
			HDITEM* item = pNMHeader->pitem;
			OnColumnEndDrag( pNMHeader->iItem, item->iOrder);
			break;
		}
	case HDN_ENDTRACK:
		{
			NMHEADER* pNMHeader = (LPNMHEADER)lParam;
			HDITEM* item = pNMHeader->pitem;			
			OnColumnEndTrack(pNMHeader->iItem, item->cxy);
			break;
		}
    }

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CLayoutListCtrl::OnColRClick(int nCol,const CPoint& point)
{
	CPoint pointOnScreen(point);
	ClientToScreen(&pointOnScreen);
	m_headerMenu.TrackPopupMenu(0,pointOnScreen.x,pointOnScreen.y,this);
}

ListSelection CLayoutListCtrl::GetSelection()
{
	ListSelection selection;
	UINT selectedCount = GetSelectedCount();
	int  nItem = -1;
	if (selectedCount > 0)
	{
		for (UINT i=0;i < selectedCount;i++)
		{
			nItem = GetNextItem(nItem, LVNI_SELECTED);
			selection.AddRow(nItem);
		}
	}
	return selection;
}

void CLayoutListCtrl::SelectAll()
{
	for (int i = 0; i< GetItemCount(); i++)
		SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

}

void CLayoutListCtrl::InvertSelection()
{
	
	for (int i = 0; i< GetItemCount(); i++)
	{
		if (GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			SetItemState(i, 0, LVIS_SELECTED);
		else
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CLayoutListCtrl::SelectNone()
{
	for (int i = 0; i< GetItemCount(); i++)
		SetItemState(i, 0, LVIS_SELECTED);
}


