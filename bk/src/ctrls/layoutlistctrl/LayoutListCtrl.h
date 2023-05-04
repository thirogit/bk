#ifndef __LAYOUTLISTCTRL_H__
#define __LAYOUTLISTCTRL_H__

#include <afxcmn.h>
#include <boost/range/any_range.hpp>
#include "../../uitypes/Color.h"
#include "LayoutHeaderCtrl.h"
#include "ListHeaderDescriptor.h"
#include "ListHeaderLayout.h"
#include <vector>
#include <map>
#include "../../compare/SortAttributes.h"

typedef boost::any_range<int, boost::forward_traversal_tag, int, std::ptrdiff_t> SelectionRange;

class ListSelection
{
public:
	ListSelection();
	ListSelection(const ListSelection& src);
	ListSelection& operator=(const ListSelection& right);
	size_t GetCount() const;
	void AddRow(int row);
	SelectionRange GetRange();
private:	
	std::vector<int> m_rows;
};

class CellDescriptor
{
public:
	CellDescriptor(int row,int col,const CRect& rect);
	int GetRow() const;
	int GetCol() const;
	const CRect& GetRect() const;

private:
	CRect m_rect;
	int m_row;
	int m_col;
};

class CLayoutListCtrl : public CListCtrl
{

public:
	CLayoutListCtrl();
	CLayoutListCtrl(const ListHeaderDescriptor& headerDescriptor);
	virtual ~CLayoutListCtrl();

	void SetHeaderDescriptor(const ListHeaderDescriptor& headerDescriptor);
	void SetHeaderLayout(ListHeaderLayout* pHeaderLayout);
		
	void SetOnRightClick(std::function<void (int,uint32_t,CellDescriptor& cell)>& handler);

	template<typename F>
	void SetOnRightClick(F& f)
	{
		std::function<void (int,uint32_t,CellDescriptor& cell)> handler = f;
		SetOnRightClick(handler);
	}

	void SetCellDblClickHandler(std::function<void (int,uint32_t,CellDescriptor& cell)>& handler);
	void SetRowDblClickHandler(std::function<void (int)>& handler);

	template<typename F>
	void SetRowDblClickHandler(F& f)
	{
		std::function<void(int)> handler = f;
		SetRowDblClickHandler(handler);
	}

	void SetCellTextResolver(std::function<std::wstring (int,uint32_t)>& resolver);
	void SetCellColorResolver(std::function<Color (int,uint32_t)>& resolver);	
	void SetCellImageResolver(std::function<int (int,uint32_t)>& resolver);	
		
	int GetCurSel();
	ListSelection GetSelection();
	int GetColumnCount();
	uint32_t GetColId(int nColPos);
	void Lock();
	void Unlock();

	void SelectAll();
	void InvertSelection();
	void SelectNone();

protected:
	virtual void BuildHeader();
			void BuildHeaderMenu();
			void AppendColumn(uint32_t colId);
			void RemoveColumn(uint32_t colId);

	virtual void PostCreate();
			void PreSubclassWindow();	
			CRect GetCellRect(int Item, int Column);
	virtual void OnColClick(int nCol);
	virtual void SortByCol(uint32_t columnId);
	virtual void SortBy(const SortAttributes& attributes);
	virtual Color GetCellColor(int nItem,int nSubItem);
	virtual std::wstring GetCellText(int nItem,int nCol);
	virtual int  GetItemImage(int iItem,int Column);
	int GetColumnOrder(int ColumIndex);
	virtual void OnColRClick(int nCol,const CPoint& point);    
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void	RepaintSortArrows();
	void OnColumnEndDrag(int iItem, int iOrder);
	void OnColumnEndTrack(int iItem, int width);

protected:
	ListHeaderLayout* m_pColumnLayout;
	ListHeaderLayout m_LocalLayout;
	ListHeaderDescriptor m_HeaderDesc;
	CLayoutHeaderCtrl m_header;
	CMenu m_headerMenu;
	SortAttributes m_sortAttributes;
	bool m_shiftPressLastSort;
	CImageList m_sortArrows;

	std::function<void (int,uint32_t,CellDescriptor& cell)>  m_CellDblClickHandler;
	std::function<void (int)> m_RowDblClickHandler;
	std::function<std::wstring (int,uint32_t)> m_CellTextResolver;
	std::function<Color (int,uint32_t)> m_CellColorResolver;
	std::function<int (int,uint32_t)> m_CellImageResolver;
	std::function<void (int,uint32_t,CellDescriptor& cell)> m_RightClickHandler;

protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderMenu(UINT nId);
	
	
	DECLARE_MESSAGE_MAP()
};

#endif