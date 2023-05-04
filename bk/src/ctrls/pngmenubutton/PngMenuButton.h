#ifndef __PNGMENUBUTTON_H__
#define __PNGMENUBUTTON_H__

#include "../../image/Png.h"
#include "../../ui/menu/MenuBuilder.h"

class CPngMenuBtn : public CButton
{
public:
	CPngMenuBtn();
	virtual ~CPngMenuBtn();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetImage(UINT nResPNG);
	void SetMenu(const MenuBuilder& menu);


protected:
	MenuBuilder*  m_menu;
	CPng		 m_btnImage;
	
	BOOL         m_bPushed;
	BOOL         m_bMenuPushed;

	CWnd*        m_pParentWnd;

	COLORREF	 m_crMask;
	BOOL         m_IsMenuEnabled;

	

protected:
	void DrawArrow(CDC* pDC, CPoint ArrowTip);
	BOOL HitMenuBtn(CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	void CreateMenu();
	void DrawImage(Gdiplus::Graphics& graphics, const CRect& drawRect);
	
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnStyleChanged(int styleType, LPSTYLESTRUCT style);
	DECLARE_MESSAGE_MAP()
};

#endif 
