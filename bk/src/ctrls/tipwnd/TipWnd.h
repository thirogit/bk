#ifndef __TIPWND_H__
#define __TIPWND_H__

#include <afxwin.h>
#include "TipWndStyle.h"

class CTipWnd : public CWnd
{
public:
	CTipWnd();	
	~CTipWnd();
	
protected:
	
	COLORREF m_rgbTextColour;
	COLORREF m_rgbBorderOuter;
	COLORREF m_rgbBorderInner;
	COLORREF m_rgbBackgroundTop;
	COLORREF m_rgbBackgroundBottom;
	
	CFont m_fntTitleFont;

	CWnd* m_pParent;
	CPoint m_Position;
	bool m_ShowCentered;
	std::wstring m_text;
	bool m_bShowing;
	CSize m_targetSize;
	CSize m_currentSize;
	int m_lastDivRemainder;
	int m_margin;
	NullInt m_fontHeight;
	
	BOOL RegisterWndClass(HINSTANCE hInstance);
public:
	BOOL Create( CWnd* pParent );
	void Show();	
	void Hide();
	void SetStyle(const TipWndStyle& style);
	void SetText(const std::wstring& text);
	void SetPosition(const CPoint& point);
	void SetShowCenteredInParent();
	
protected:
	UINT HitTest(CPoint &ptClient);
	void OnMouseMessage(UINT message,UINT ncMessage,UINT nFlags,CPoint point);
	
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nTimerID);


	DECLARE_MESSAGE_MAP();

};
#endif