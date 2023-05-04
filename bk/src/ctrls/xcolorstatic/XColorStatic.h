// XColorStatic.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XCOLORSTATIC_H
#define XCOLORSTATIC_H

/////////////////////////////////////////////////////////////////////////////
// CXColorStatic window

class CXColorStatic : public CStatic
{
// Construction
public:
	CXColorStatic();
	virtual ~CXColorStatic();

// Attributes
public:
	void SetBackgroundColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetTextColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetBold(BOOL bFlag, BOOL bRedraw = TRUE);
	void SetFont(LPCTSTR lpszFaceName, int nPointSize, BOOL bRedraw = TRUE);
	void SetFont(LOGFONT * pLogFont, BOOL bRedraw = TRUE);
	void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	void SetIcon(HICON hIcon, BOOL bRedraw = TRUE);
	void SetPadding(int x, int y);
	void SetPadding(int l, int r, int t, int b);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorStatic)
protected:
    virtual void PreSubclassWindow();
	void DestroyObjects();
	void  CreateObjects();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont*		m_pFont;
	COLORREF	m_rgbText;
	COLORREF	m_rgbBackground;
	CBrush *	m_pBrush;
	BOOL		m_bBold;
	CRect		m_Padding;
	HICON		m_hIcon;

	// Generated message map functions
protected:
	//{{AFX_MSG(CXColorStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XCOLORSTATIC_H
