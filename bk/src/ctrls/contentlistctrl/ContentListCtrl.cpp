#include "stdafx.h"
#include "ContentListCtrl.h"


BEGIN_MESSAGE_MAP(CContentListCtrl,CLayoutListCtrl)
	
	ON_WM_DESTROY()
	
END_MESSAGE_MAP()

CContentListCtrl::CContentListCtrl() : m_pContent(NULL)
{
}

void CContentListCtrl::OnDestroy()
{	
	if(m_pContent != NULL)
		m_pContent->RemoveContentObserver(this);
	CLayoutListCtrl::OnDestroy();
}

void CContentListCtrl::SetContent(ListContent* pContent)
{
	if(m_pContent != NULL)
		m_pContent->RemoveContentObserver(this);
		
	m_pContent = pContent;

	if(m_pContent != NULL)
		m_pContent->AddContentObserver(this);

	UpdateItemCount();
}

void CContentListCtrl::UpdateItemCount()
{
	int rowCount = 0;
	if(m_pContent)
		rowCount = m_pContent->GetRowCount();

	if(::IsWindow(m_hWnd))
	{
		SetItemCount(rowCount);
	}

}

std::wstring CContentListCtrl::GetCellText(int row,uint32_t colId)
{
	std::wstring text;
	if(m_pContent)
	{
		ListRow* pRow = m_pContent->GetRow(row);
		text = pRow->CellValue(colId);
	}
	return text;
}

Color CContentListCtrl::GetCellColor(int row,uint32_t colId)
{
	Color color;
	if(m_pContent)
	{
		ListRow* pRow = m_pContent->GetRow(row);
		color = pRow->CellColor(colId);
	}
	return color;		
}

int CContentListCtrl::GetCellImage(int row, uint32_t colId)
{
	int image = -1;
	if (m_pContent)
	{
		ListRow* pRow = m_pContent->GetRow(row);
		image = pRow->CellImage(colId);
	}
	return image;
}

void CContentListCtrl::PostCreate()
{
	CLayoutListCtrl::PostCreate();

	std::function<std::wstring (int,uint32_t)> textresolver = 
		std::bind(&CContentListCtrl::GetCellText,this,std::placeholders::_1,std::placeholders::_2);

	std::function<Color (int,uint32_t)> clrresolver = 
		std::bind(&CContentListCtrl::GetCellColor,this,std::placeholders::_1,std::placeholders::_2);

	std::function<int (int, uint32_t)> imgresolver =
		std::bind(&CContentListCtrl::GetCellImage, this, std::placeholders::_1, std::placeholders::_2);
	
	SetCellTextResolver(textresolver);
	SetCellColorResolver(clrresolver);
	SetCellImageResolver(imgresolver);
	UpdateItemCount();
}

CContentListCtrl::~CContentListCtrl()
{
	
}


void CContentListCtrl::OnBeforeUpdate()
{
	Lock();
}

void CContentListCtrl::OnCountChanged()
{
	UpdateItemCount();
}

void CContentListCtrl::OnContentChanged()
{
	Invalidate(FALSE);
}

void CContentListCtrl::OnAfterUpdate()
{
	Unlock();
}

void CContentListCtrl::SortBy(const SortAttributes& attributes)
{
	if(m_pContent)
		m_pContent->SortContent(attributes);
}

ListContent* CContentListCtrl::GetContent()
{
	return m_pContent;
}