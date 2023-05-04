#ifndef __VNAVIGATION_H__
#define __VNAVIGATION_H__

#include <vector>
#include <functional>

struct TabLook
{
	COLORREF hot;
	COLORREF active;
	COLORREF notactive;
};

class CVNavigation : public CStatic
{
	
	class Tab : public CStatic
	{
		public:
		Tab(CVNavigation* parent);
		BOOL Create(const std::wstring& caption, const CRect& rect, CWnd* pParentWnd, UINT id);
		void SetLook(const TabLook* look);
		
		std::function<void(UINT)> m_onSelected;

		private:
		CVNavigation* m_parent;
	
		CBrush 	m_FgBrush;
		CBrush 	m_HotBrush;
		CBrush 	m_SelectedBrush;
		BOOL m_bMouseTracking;
		BOOL m_bMouseOver;
		BOOL m_bLBtnDown;
		protected:
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);

		afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
		DECLARE_MESSAGE_MAP()
	};
	
	friend class Tab;
	
public:
	CVNavigation();
	virtual ~CVNavigation();
	UINT AddTab(const std::wstring& tabText,std::function<void (UINT)>& onSelected,const TabLook* look = NULL);
	
public:
	
protected:
    virtual void PreSubclassWindow();	
	void DestroyObjects();
	void  CreateObjects();	
	int CalculateHeight();
	void SelectTab(Tab* tab);
	bool IsSelected(Tab* tab);
	
protected:
	CFont		m_Font;
	CFont		m_SelectedFont;
	CBrush *	m_pBkgBrush;
	std::vector<Tab*> m_tabs;
	UINT		m_idSequence;
	Tab*		m_pSelectedTab;
protected:
	
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	

	DECLARE_MESSAGE_MAP()
};


#endif //VNAVIGATION_H
