#if !defined(AFX_XEDIT_H__22DE2BCC_1D68_484C_9B71_26FAA6142B98__INCLUDED_)
#define AFX_XEDIT_H__22DE2BCC_1D68_484C_9B71_26FAA6142B98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../ctrls/stdedit/stdedit.h"

void UpdateCaret(CEdit*, BOOL = TRUE);
void DeleteNextChar(CEdit*);

/////////////////////////////////////////////////////////////////////////////
// CXEdit window

class CXEdit : public CStdEdit
{
// Construction
public:
	CXEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bInsert;
	virtual ~CXEdit();
	virtual void SetText(const std::wstring& sText);
	BOOL WasModified();
	void Clear();
protected:
	BOOL m_bModified;
	CBrush m_backColor;
	// Generated message map functions
protected:
	//{{AFX_MSG(CXEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//void OnChangeReflect(NMHDR * nmhdr,LRESULT *lResult);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XEDIT_H__22DE2BCC_1D68_484C_9B71_26FAA6142B98__INCLUDED_)
