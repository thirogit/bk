#ifndef __EXPANDABLELISTCTRL_H__
#define __EXPANDABLELISTCTRL_H__

#include <afxwin.h>
#include "ListHeaderDescriptor.h"
#include "ListHeaderLayout.h"
#include "ListRow.h"
#include <vector>
#include <set>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include "../OwnClassWnd.h"

class CExpandableListCtrl : public COwnClassWnd
{	

	struct CHeader : public CStatic
	{
		CExpandableListCtrl* m_parent;

		void Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);

		DECLARE_MESSAGE_MAP()
	};


	struct CBody : public CStatic
	{
		CExpandableListCtrl* m_parent;

		void Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

		DECLARE_MESSAGE_MAP()
	};

	struct CRow : public CStatic
	{
		CExpandableListCtrl* m_parent;
		CWnd* m_content;
		ListRow* m_row;
		
		CPen m_GridPen;
		bool m_bLBtnDown;
		bool m_bExpanded;
		bool m_bSelected;

		void Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
		void DrawHTriangle(CDC* pDC, int x, int y);
		void DrawVTriangle(CDC* pDC, int x, int y);
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

		DECLARE_MESSAGE_MAP()
	};

	struct CColumn
	{
		std::wstring text;
		uint32_t id;
		uint32_t width;
	};

	struct index_tag_id {};


	struct RowKeyExtractor
	{
		typedef int result_type;
		result_type operator()(CRow* row) const
		{
			return row->GetDlgCtrlID();
		}
	};

	typedef boost::multi_index_container
		<
		CRow*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, RowKeyExtractor >
		>
		> RowsIndex;

public:
	CExpandableListCtrl();
	
	void SetHeader(const ListHeaderDescriptor& header);
	void SetHeaderLayout(const ListHeaderLayout& layout);
	
	void InsertColumn(const std::wstring& sColumnText,uint32_t id,uint32_t width);	
	void AddRow(ListRow* row,CWnd* pContent);
	
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
private:
	int ComputeHeight();
	int ComputeWidth();
	int GetWindowH(CWnd* pWnd);
	virtual void PreSubclassWindow();
	void CreateHeader();
	void Expand(CRow* row);
	void Select(CRow* row);
	void CreateScrollBars();
	void CreateBody();
	void UpdateScrollBars(int height);

	bool IsExpanded(CRow* row);
	bool IsSelected(CRow* row);
	void RepositionRowsBelow(CRow* row,int deltaY);
	CPoint GetRowPosition(CRow* row);
	int GetScrollPos(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	RowsIndex m_rows;
	std::vector<CColumn> m_columns;
	CBrush	m_BkgBrush;
	CBrush	m_BkgDebugBrush;
	UINT m_idSequence;
	CHeader m_header;
	CRow* m_rowSelected;
	CBody m_body;
	CScrollBar m_vScrollBar;
	int m_scrollYPos;
	int m_maxScrollY;
	int m_scrollY;
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()


	friend struct CRow;
};


#endif

