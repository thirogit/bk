#pragma once
#include "TabPage.h"
#include "..\ctrls\contentlistctrl\ContentListCtrl.h"

class ListTab : public TabPage
{
public:
	ListTab();
	BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
	void SetContent(ListContent *content);
	ListContent* GetContent();

	void SetHeaderLayout(ListHeaderLayout* pHeaderLayout);

	void SetHeaderDescriptor(const ListHeaderDescriptor& headerDescriptor);

	template<typename F>
	void SetOnDblClick(F& f)
	{
		std::function<void (int,int)> handler = f;
		SetOnDblClick(handler);
	}
	
	void SetOnDblClick(std::function<void (int,int)>& handler);

	template<typename F>
	void SetOnRightClick(F& f)
	{
		std::function<void (int,int)> handler = f;
		SetOnRightClick(handler);
	}

	void SetOnRightClick(std::function<void (int,int)>& handler);

	ListSelection GetSelection();
	int GetSelectedRow();
	void SelectAll();
	void InvertSelection();
	virtual ~ListTab();
protected:
	CContentListCtrl m_List;	

	std::function<void (int,int)> m_RClickHandler;
	std::function<void (int,int)> m_DblClickHandler;
protected:	
	void OnRightClick(int row,uint32_t colId,CellDescriptor& cell);
	void OnDblClick(int row,uint32_t colId,CellDescriptor& cell);




	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize( UINT nType,  int cx,  int cy  );	
	DECLARE_MESSAGE_MAP()
};

