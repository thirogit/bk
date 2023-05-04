#ifndef __ENTEREDIT_H__
#define __ENTEREDIT_H__

#include "../FilterEdit/FilterEdit.h"

#define EN_EDITCOMPLETE (WM_APP + 100)
#define UNCOMMITED_CLR RGB(239,134,134)

template<class BASE>
class CWrapperEnterEdit : public BASE
{
	struct CTLCOLOR
	{
		HWND hWnd;
		HDC hDC;
		UINT nCtlType;
	};
public:
	CWrapperEnterEdit() : m_bUncommited(false)
	{
	}

	~CWrapperEnterEdit()
	{
		DeleteBackgroundBrush();
	}

	bool IsUncommited()
	{
		return m_bUncommited;
	}

	void CommitText()
	{
		m_bUncommited = false;
		Invalidate(FALSE);
	}

	bool IsTextChanged()
	{
		CString wndText;
		GetWindowText(wndText);
		return (wndText != m_prevText);
	}


protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		*pResult = 0;
		switch(message)
		{
			case WM_GETDLGCODE:
				*pResult = OnGetDlgCode();
				return TRUE;
			case WM_CHAR:
				OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
				return TRUE;
			case WM_CREATE:
				*pResult = OnCreate(reinterpret_cast<LPCREATESTRUCT>(lParam));				
				return TRUE;
			case WM_KEYDOWN:
				OnKeyDown(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
				return TRUE;			
		}

		return BASE::OnWndMsg(message,wParam,lParam,pResult);
	}
	
	void CreateBackgroudBrush()
	{
		DeleteBackgroundBrush();
		m_brBackGnd.CreateSolidBrush(UNCOMMITED_CLR);
	}
	void DeleteBackgroundBrush()
	{
		if (m_brBackGnd.GetSafeHandle())
		   m_brBackGnd.DeleteObject();
	}


	UINT OnGetDlgCode() 
	{
		return BASE::OnGetDlgCode() | DLGC_WANTALLKEYS;
	}

	void OnReturnKey()
	{		
		NMHDR hdr;
		hdr.code = EN_EDITCOMPLETE;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		CWnd* pParent = GetParent();
		pParent->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
	}

	
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		BASE::OnKeyDown(nChar, nRepCnt, nFlags);

		if(nChar == VK_DELETE)
		{
			if(IsTextChanged() && !m_bUncommited)
					m_bUncommited = true;
			Invalidate(FALSE);
		}
	}

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{	
		switch(nChar)
		{
			case VK_RETURN:
				OnReturnKey();
				GetWindowText(m_prevText);
				m_bUncommited = false;
				break;
			case VK_TAB:
			{
				CWnd *pNextWnd = GetNextWindow();
				if(pNextWnd)
					pNextWnd->SetFocus();			
				break;
			}
			default:
				BASE::OnChar(nChar, nRepCnt, nFlags);
				if(IsTextChanged() && !m_bUncommited)
					m_bUncommited = true;
				break;
		}
		Invalidate(TRUE);

	}

	
	virtual bool OnSetText(const TCHAR *szText)
	{
		if(m_prevText.Compare(szText) != 0 && !m_bUncommited)
			m_bUncommited = true;
		return BASE::OnSetText(szText);
	}

	int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		CreateBackgroudBrush();
		return BASE::OnCreate(lpCreateStruct);	
	}

	BOOL OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
		{					
			BOOL bResult = TRUE;
			CDC dcTemp; 
			dcTemp.m_hDC = (HDC)wParam;
			UINT nCtlType = uMsg - WM_CTLCOLORMSGBOX;
			HBRUSH hbr = CtlColor(&dcTemp, nCtlType);
			dcTemp.m_hDC = NULL;
			*pResult = reinterpret_cast<LRESULT>(hbr);
			
			if ((HBRUSH)*pResult == NULL) bResult = FALSE;
			return bResult;
		}
		else
		{
			return BASE::OnChildNotify(uMsg, wParam, lParam, pResult);
		}
	}

	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor) 
	{
		if(m_bUncommited)
		{
			pDC->SetBkColor(UNCOMMITED_CLR);
			return (HBRUSH)m_brBackGnd.GetSafeHandle();
		}
		return (HBRUSH)NULL;
	}

	virtual void PreSubclassWindow()
	{
		BASE::PreSubclassWindow();
		CreateBackgroudBrush();
	}
		
public:
	void SetCommitedText(LPCTSTR szText)
	{
		m_bUncommited = false;
		m_prevText = szText;
		SetWindowText(szText);
	}

private:
	COLORREF m_crBackGnd;
	CBrush m_brBackGnd;	
	CString m_prevText;	
	bool m_bUncommited;
};


class CEnterEdit : public CFilterEdit
{
public:
	CEnterEdit();
	~CEnterEdit();
	bool IsUncommited();
	virtual void SetCommitedText(LPCTSTR szText);
protected:
	virtual void OnReturnKey();

	COLORREF m_crBackGnd;
	CBrush m_brBackGnd;
	
	CString m_prevText;
	
	bool m_bUncommited;

	void CreateBackgroudBrush();
	void DeleteBackgroundBrush();
	virtual void PreSubclassWindow();
	virtual bool OnSetText(const TCHAR *szText);

	BOOL OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	bool IsTextChanged();
	
protected:
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};

#endif 
