#include "stdafx.h"
#include "ClickableGroupBox.h"

#define ID_TITLE	1001

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( CClickableGroupBox, CButton )

CClickableGroupBox::CClickableGroupBox()
{
}

CClickableGroupBox::~CClickableGroupBox()
{
}

BEGIN_MESSAGE_MAP(CClickableGroupBox, CButton)
	ON_WM_ENABLE()
	ON_WM_SETFOCUS()
	ON_COMMAND( ID_TITLE , OnClicked)	
END_MESSAGE_MAP()

void CClickableGroupBox::PreSubclassWindow()
{
	CButton::PreSubclassWindow();

	CString strText;
	GetWindowText(strText);

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(GetFont());
	CSize czText = dc.GetTextExtent(strText);
	dc.SelectObject(pOldFont);
	// Add some space for the checkbox and at the end
	czText.cx += 25;

	// Move the checkbox on top of the groupbox
	CRect rc;
	GetWindowRect(rc);	
	this->ScreenToClient(rc);
	rc.left += 5;
	rc.right = rc.left + czText.cx;
	rc.bottom = rc.top + czText.cy + 4;

	m_TitleBtn.Create(strText, BS_PUSHBUTTON | WS_CHILD  , rc, this, ID_TITLE);
	m_TitleBtn.SetFont(GetFont(), true);
	m_TitleBtn.ShowWindow(SW_SHOW);
	
}

void CClickableGroupBox::OnClicked() 
{
	
	::SendMessage(GetParent()->m_hWnd,
					WM_COMMAND, 
					MAKEWPARAM(::GetDlgCtrlID(m_hWnd), BN_CLICKED),
					(LPARAM)m_hWnd);
	
}


void CClickableGroupBox::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	m_TitleBtn.EnableWindow(bEnable);

}

void CClickableGroupBox::OnSetFocus(CWnd* pOldWnd) 
{
	m_TitleBtn.SetFocus();
}

