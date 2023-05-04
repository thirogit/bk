#include "stdafx.h"
#include "ListTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_LIST 1001

ListTab::ListTab()
{
}

ListTab::~ListTab()
{
}

BEGIN_MESSAGE_MAP(ListTab, TabPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void ListTab::OnSize( UINT nType,  int cx,  int cy  )
{
	TabPage::OnSize(nType,cx,cy);

	m_List.SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);

}

void ListTab::SetContent(ListContent *content)
{
	m_List.SetContent(content);
}

ListContent* ListTab::GetContent()
{
	return m_List.GetContent();
}

BOOL ListTab::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	return TabPage::Create( dwStyle, rect, pParentWnd, nID);
}

int ListTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(TabPage::OnCreate(lpCreateStruct) != 0)
		return -1;
	

	if(!m_List.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | LVS_OWNERDATA | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
		              CRect(),
					  this,
					  ID_LIST))
	{
		return -1;
	}

	std::function<void (int,uint32_t,CellDescriptor& cell)> RClickFn = 
		std::bind(&ListTab::OnRightClick,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	m_List.SetOnRightClick(RClickFn);

	std::function<void (int,uint32_t,CellDescriptor& cell)> DblClickFn = 
		std::bind(&ListTab::OnDblClick,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

	m_List.SetCellDblClickHandler(DblClickFn);
	
	return 0;
}

void ListTab::SetOnDblClick(std::function<void (int,int)>& handler)
{
	m_DblClickHandler = handler;
}

void ListTab::SetOnRightClick(std::function<void (int,int)>& handler)
{
	m_RClickHandler = handler;
}

void ListTab::OnRightClick(int row,uint32_t colId,CellDescriptor& cell)
{
	if(m_RClickHandler)
		m_RClickHandler(cell.GetRow(),cell.GetCol());
}

void ListTab::OnDblClick(int row,uint32_t colId,CellDescriptor& cell)
{
	if(m_DblClickHandler)
		m_DblClickHandler(cell.GetRow(),cell.GetCol());	
}

ListSelection ListTab::GetSelection()
{
	return m_List.GetSelection();
}

int ListTab::GetSelectedRow()
{
	return m_List.GetCurSel();
}

void ListTab::SetHeaderDescriptor(const ListHeaderDescriptor& headerDescriptor)
{
	m_List.SetHeaderDescriptor(headerDescriptor);
}

void ListTab::SetHeaderLayout(ListHeaderLayout* pHeaderLayout)
{
	m_List.SetHeaderLayout(pHeaderLayout);
}

void ListTab::SelectAll()
{
	m_List.SelectAll();
}

void ListTab::InvertSelection()
{
	m_List.InvertSelection();
}

