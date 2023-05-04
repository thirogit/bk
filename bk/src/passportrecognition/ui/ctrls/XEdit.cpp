#include "stdafx.h"
#include "XEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BACKGROUNDCOLOR RGB(255,255,128)

void UpdateCaret(CEdit* pEdit, BOOL bInsert)
{
	CFont* pFont = pEdit->GetFont();
	CDC* pDC = pEdit->GetDC();
	CFont* pOldFont = pDC->SelectObject(pFont);
	CSize sizeChar = pDC->GetTextExtent(_T("0"));
	//if (bInsert)
	//{
		// Insert mode, vertical line caret
	///	sizeChar.cx = 0;
	//}
	//else
	{
		// Checks whether caret is at the end of current line
		int nLineIndex = pEdit->LineIndex();
		int nLineLength = pEdit->LineLength();
		int nStart, nEnd;
		pEdit->GetSel(nStart, nEnd);
		if (nStart == nEnd && nStart != nLineIndex + nLineLength)
		{
			// No text selected & caret is not at end of line
			// So, gets next character
			TCHAR* strLine = new TCHAR[nLineLength + 1];
			pEdit->GetLine(pEdit->LineFromChar(), strLine, nLineLength);
			strLine[nStart - nLineIndex + 1] = 0;
			// Sets caret size as size of next character
			sizeChar = pDC->GetTextExtent(strLine + (nStart - nLineIndex));
			delete[] strLine;
		}
	}
	// Destroys previous caret and displays new caret
	DestroyCaret();
	pEdit->HideCaret();
	pEdit->CreateSolidCaret(sizeChar.cx, sizeChar.cy);
	pEdit->ShowCaret();
	// Stores caret width in application object
//	((CIOEditApp*) AfxGetApp())->m_nCaretWidth = sizeChar.cx;
	pDC->SelectObject(pOldFont);
}

void DeleteNextChar(CEdit* pEdit)
{
	int Start, End;
	pEdit->GetSel(Start, End);
	pEdit->ReplaceSel(_T(""));
	pEdit->SetSel(Start, End + 1);
	pEdit->ReplaceSel(_T(""));
}

/////////////////////////////////////////////////////////////////////////////
// CXEdit

CXEdit::CXEdit() : m_bModified(FALSE),m_bInsert(TRUE),m_backColor(BACKGROUNDCOLOR)
{
}

CXEdit::~CXEdit()
{
}


BEGIN_MESSAGE_MAP(CXEdit, CEdit)
	//{{AFX_MSG_MAP(CXEdit)
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR_REFLECT() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXEdit message handlers

BOOL CXEdit::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case (WM_KEYDOWN):
	case (WM_KEYUP):
	case (WM_LBUTTONDOWN):
	case (WM_LBUTTONUP):
	case (WM_SETFOCUS):
		{
			UpdateCaret(this, m_bInsert);
			break;
		}
	case (WM_CHAR):
		{			
			if (!m_bInsert)
				DeleteNextChar(this);
			if(!m_bModified)
			{
				m_bModified = TRUE;
				Invalidate();
			}
			
			break;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CXEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int Start, End;
	 
	switch(nChar)
	{
		case VK_INSERT:
			m_bInsert = !m_bInsert;
		break;
		case VK_DELETE:
			GetSel(Start, End);
			if(End < GetWindowTextLength() && !m_bModified)
			{
				m_bModified = true;
				Invalidate();
			}
		break;
	}
	

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXEdit::SetText(const std::wstring& sText)
{
	CStdEdit::SetText(sText);
	m_bModified = FALSE;
}

BOOL CXEdit::WasModified()
{
	return m_bModified;
}

void CXEdit::Clear()
{
	SetWindowText(L"");
	m_bModified = FALSE;

}
HBRUSH CXEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if(m_bModified)
	{
		pDC->SetBkColor(BACKGROUNDCOLOR);
        return (HBRUSH)m_backColor.GetSafeHandle();
	}
	return NULL;
}