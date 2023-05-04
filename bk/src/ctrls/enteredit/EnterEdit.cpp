// EnterEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EnterEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CEnterEdit::CEnterEdit() : m_bUncommited(false)
{
}


CEnterEdit::~CEnterEdit()
{
	DeleteBackgroundBrush();
}


BEGIN_MESSAGE_MAP(CEnterEdit, CFilterEdit)
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CEnterEdit::CreateBackgroudBrush()
{
	DeleteBackgroundBrush();
	m_brBackGnd.CreateSolidBrush(UNCOMMITED_CLR);
}
void CEnterEdit::DeleteBackgroundBrush()
{
	if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
}


UINT CEnterEdit::OnGetDlgCode() 
{
    return CFilterEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CEnterEdit::OnReturnKey()
{		
	NMHDR hdr;
	hdr.code = EN_EDITCOMPLETE;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();
	CWnd* pParent = GetParent();
	pParent->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
}

bool CEnterEdit::IsUncommited()
{
	return m_bUncommited;
}

void CEnterEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CFilterEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	if(nChar == VK_DELETE)
	{
		if(IsTextChanged() && !m_bUncommited)
				m_bUncommited = true;
		Invalidate(FALSE);
	}
}

void CEnterEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
			CFilterEdit::OnChar(nChar, nRepCnt, nFlags);
			if(IsTextChanged() && !m_bUncommited)
				m_bUncommited = true;
			break;
	}
	Invalidate(TRUE);
	
}

bool CEnterEdit::IsTextChanged()
{
	CString wndText;
	GetWindowText(wndText);
	return (wndText != m_prevText);
}

void CEnterEdit::SetCommitedText(LPCTSTR szText)
{
	m_bUncommited = false;
	m_prevText = szText;
	SetWindowText(szText);
}
bool CEnterEdit::OnSetText(const TCHAR *szText)
{
	if(m_prevText.Compare(szText) != 0 && !m_bUncommited)
		m_bUncommited = true;
	return CFilterEdit::OnSetText(szText);
}

int CEnterEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CreateBackgroudBrush();
	return CFilterEdit::OnCreate(lpCreateStruct);	
}

HBRUSH CEnterEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if(m_bUncommited)
	{
		pDC->SetBkColor(UNCOMMITED_CLR);
		return (HBRUSH)m_brBackGnd.GetSafeHandle();
	}
	return (HBRUSH)NULL;
}

void CEnterEdit::PreSubclassWindow()
{
	CFilterEdit::PreSubclassWindow();
	CreateBackgroudBrush();
}


BOOL CEnterEdit::OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CFilterEdit::OnChildNotify(uMsg, wParam, lParam, pResult);


}
