#ifndef __NUMERICEDIT_H__
#define __NUMERICEDIT_H__

#include "../stdedit/stdedit.h"
#include <boost/regex.hpp>

class CNumericEdit : public CStdEdit
{

public:
	CNumericEdit (int precision);
	virtual ~CNumericEdit ();

	bool IsInputEmpty();
		
protected:
	virtual bool IsValid (const TCHAR *pszText);
	bool ValidateChar (UINT nChar);
	bool Match (const TCHAR *pszText);
	
	bool CanPaste ();
	
	virtual bool OnSetText(const TCHAR *szText);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

private:
	bool m_bControlDown;	
	boost::wregex m_floatregex;
	
};


#endif 
