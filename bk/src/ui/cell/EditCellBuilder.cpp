#include "stdafx.h"
#include "EditCellBuilder.h"


#define IDC_EDITCELL 9004

BEGIN_MESSAGE_MAP(CEditCell,CFilterEdit)
  
    ON_WM_KILLFOCUS()
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_CREATE()
    ON_WM_GETDLGCODE()
	
END_MESSAGE_MAP()


void CEditCell::SendText()
{
	if(m_txtrecver)
		m_txtrecver(GetText());
}


void CEditCell::SetRegex(const std::wstring& sRegex)
{
	SetRegEx(sRegex.c_str());
}

void CEditCell::SetTextReceiver(TextReceiver& txtRecv)
{
	m_txtrecver = txtRecv;
}

BOOL CEditCell::PreTranslateMessage (MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
	    if (pMsg->wParam == VK_RETURN || /*pMsg->wParam == VK_DELETE || */
			pMsg->wParam == VK_ESCAPE || /*pMsg->wParam == VK_TAB || 
			pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN ||*/ GetKeyState (VK_CONTROL))
			{
				::TranslateMessage (pMsg);
				::DispatchMessage (pMsg);
				return TRUE;		    	// DO NOT process further
			}
    }

    return CFilterEdit::PreTranslateMessage (pMsg);
}


void CEditCell::OnKillFocus (CWnd* pNewWnd) 
{
    CFilterEdit::OnKillFocus(pNewWnd);
    DestroyWindow();
}


void CEditCell::OnNcDestroy() 
{
    CFilterEdit::OnNcDestroy();
    delete this;
}

void CEditCell::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    //BOOL Shift = GetKeyState (VK_SHIFT) < 0;
    switch (nChar)
    {
		case VK_RETURN :
			SendText();
		case VK_ESCAPE :
			GetParent()->SetFocus();
			return;
   }
    CFilterEdit::OnChar (nChar, nRepCnt, nFlags);
}

int CEditCell::OnCreate (LPCREATESTRUCT lpCreateStruct) 
{
    if (CFilterEdit::OnCreate (lpCreateStruct) == -1)
		return -1;

    // Set the proper font
	CFont* Font = GetParent()->GetFont();
    SetFont (Font);

    //SetWindowText (InitText);
    
	SetFocus();
    
    return 0;
}

UINT CEditCell::OnGetDlgCode() 
{
    return CFilterEdit::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
int CEditCell::Create(const RECT &rect,CWnd *pParentWnd,UINT nID)
{
	return CFilterEdit::Create(ES_LEFT | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,rect,pParentWnd,nID);
}

//-------------------------------------------------------------------------------------------------

EditCellBuilder::EditCellBuilder()
{
}

EditCellBuilder& EditCellBuilder::regex(const std::wstring& sRegex)
{
	m_sRegex = sRegex;
	return *this;
}

EditCellBuilder& EditCellBuilder::text(const std::wstring& sText)
{
	m_sText = sText;
	return *this;
}

EditCellBuilder& EditCellBuilder::receiver(std::function<void (const std::wstring& text)>& recver)
{
	m_recver = recver;
	return *this;
}

void EditCellBuilder::show(CWnd* pParent,const CRect& rect)
{
	CEditCell *pEdit = new CEditCell();
	pEdit->SetRegex(m_sRegex);
	pEdit->SetTextReceiver(m_recver);
	pEdit->Create (rect,pParent, IDC_EDITCELL);		
	pEdit->SetText(m_sText);
	pEdit->SetSel (0, -1);
}