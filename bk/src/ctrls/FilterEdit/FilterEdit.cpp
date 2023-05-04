#include "stdafx.h"
#include "FilterEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// NOTE: Be very careful how you specify your regular expressions.
// For strictly formatted data, you will want a rigid regex
// and rely on automatic formatting in a derived class to make
// data entry easier (see the CDateEdit example to see this approach
// in action). For easier validations (say unsigned int) a much looser
// regex can be used without problems.

/////////////////////////////////////////////////////////////////////////////
// CFilterEdit

// public functions:

CFilterEdit::CFilterEdit () :
	m_bProcessChars (true),
	m_bRegExOwner (true),
	m_pRegEx (0)
{
	SetDefaults ();
}

CFilterEdit::CFilterEdit (const TCHAR *pszRegEx,
	const bool bProcessChars/* = true*/) :
	m_bProcessChars (bProcessChars),
	m_bRegExOwner (true),
	m_pRegEx (0)
{
	SetDefaults ();
	SetRegEx (pszRegEx);
}

CFilterEdit::CFilterEdit (CRegEx *pRegEx,
	const bool bProcessChars/* = true*/) :
	m_bProcessChars (bProcessChars),
	m_bRegExOwner (pRegEx == 0),
	m_pRegEx (pRegEx)
{
	SetDefaults ();
}

CFilterEdit::~CFilterEdit ()
{
	if (m_bRegExOwner)
	{
		delete m_pRegEx;
		m_pRegEx = 0;
	}
}

bool CFilterEdit::SetRegEx (const TCHAR *pszRegEx)
{
	// Only Change regex if we are the owner.
	bool bSuccess = m_bRegExOwner;

	if (bSuccess)
	{
		if (!m_pRegEx)
		{
			ConstructRegEx ();
		}

		try
		{
#ifdef FILTER_EDIT_BOOST
			m_pRegEx->assign (pszRegEx);
#else
			m_pRegEx->SetRegEx (pszRegEx);
#endif
		}
		catch (const std::exception &e)
		{
			CString strError (e.what ());

			bSuccess = false;
			strError = _T("SetRegEx failed:\r\n") + strError;
			AfxMessageBox (strError, MB_OK);
		}
	}

	return bSuccess;
}

bool CFilterEdit::IsInputEmpty()
{
	return GetWindowTextLength() == 0;
}

bool CFilterEdit::IsInputValid ()
{
	bool bSuccess = true;
	CString str;

	GetWindowText (str);
	bSuccess = IsTextValid (str);

	return bSuccess;
}

const CString &CFilterEdit::GetProposedText () const
{
	return m_strProposedText;
}

void CFilterEdit::SetProcessChars (const bool bProcessChars)
{
	m_bProcessChars = bProcessChars;
}

bool CFilterEdit::GetProcessChars () const
{
	return m_bProcessChars;
}

void CFilterEdit::SetBkOnError (const bool bSetBkOnError)
{
	m_bSetBkOnError = bSetBkOnError;
}

bool CFilterEdit::GetBkOnError () const
{
	return m_bSetBkOnError;
}

void CFilterEdit::SetWavyLineOnError (const bool bWavyLineOnError)
{
	m_bWavyLineOnError = bWavyLineOnError;
}

bool CFilterEdit::GetWavyLineOnError () const
{
	return m_bWavyLineOnError;
}

void CFilterEdit::SetBackgroundColourFocus (const COLORREF crBkFocus)
{
	m_crBkFocus = crBkFocus;
	m_brFocus.DeleteObject ();
	m_brFocus.CreateSolidBrush (m_crBkFocus);
}

void CFilterEdit::SetForegroundColourFocus (const COLORREF crFgFocus)
{
	m_crFgFocus = crFgFocus;
}

void CFilterEdit::SetBackgroundColourOK (const COLORREF crBkOK)
{
	m_crBkOK = crBkOK;
	m_brOK.DeleteObject ();
	m_brOK.CreateSolidBrush (m_crBkOK);
}

void CFilterEdit::SetForegroundColourOK (const COLORREF crFgOK)
{
	m_crFgOK = crFgOK;
}

void CFilterEdit::SetBackgroundColourError (const COLORREF crBkError)
{
	m_crBkError = crBkError;
	m_brError.DeleteObject ();
	m_brError.CreateSolidBrush (m_crBkError);
}

void CFilterEdit::SetForegroundColourError (const COLORREF crFgError)
{
	m_crFgError = crFgError;
}

void CFilterEdit::CreateToolTip (CWnd *pParent, const TCHAR *pszText,
	const int iIconType/* = 2*/, const TCHAR *pszTitle/* = _T("Invalid Data")*/)
{
	TOOLINFO ti;
	DWORD dwStyle = TTS_BALLOON;

	m_ToolTip.Create (pParent, dwStyle);
	m_ToolTip.FillInToolInfo (ti, this, 0);
	ti.uFlags |= TTF_TRACK | TTF_CENTERTIP | TTF_ABSOLUTE;
	ti.lpszText = (LPTSTR) pszText;
#if _MSC_VER >= 1310
	m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
	m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM> (&ti));
}

void CFilterEdit::SetToolTipText (const TCHAR *pszText)
{
	m_ToolTip.UpdateTipText (pszText, this);
}

void CFilterEdit::SetToolTipTitle (const int iIconType, const TCHAR *pszTitle)
{
#if _MSC_VER >= 1310
	m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
}

// protected functions:

bool CFilterEdit::IsTextValid (const TCHAR *pszText)
{
	bool bSuccess = true;

#ifdef FILTER_EDIT_BOOST
	if(m_pRegEx)
	{
		boost::match_results<const TCHAR *> what;
		bSuccess = boost::regex_match (pszText, what, *m_pRegEx,boost::match_default);
	}
#else
	bSuccess = m_pRegEx->Match (pszText);
#endif

	return bSuccess;
}

bool CFilterEdit::ValidateChar (UINT nChar)
{
	int iStartIndex = -1;
	int iEndIndex = -1;
	bool bCharValid = true;

	GetSel (iStartIndex, iEndIndex);
	GetWindowText (m_strProposedText);

	//  Backspace validation is done here as it has a virtual key code.
	if (nChar == VK_BACK)
	{
		if (iStartIndex == iEndIndex)
		{
			iStartIndex--;
		}

		// Remove char
		m_strProposedText.Delete (iStartIndex, iEndIndex - iStartIndex);
		if(m_strProposedText == "") bCharValid = true;
		else bCharValid = Match (m_strProposedText);
	}
	else
	{
		m_strProposedText.Delete (iStartIndex, iEndIndex - iStartIndex);
		m_strProposedText.Insert (iStartIndex, static_cast<TCHAR> (nChar));
		if(m_strProposedText == "") bCharValid = true;
		else bCharValid = Match (m_strProposedText);
	}

	return bCharValid;
}

// NOTE: This function checks if the given string matches SO FAR.
// To check if the edit control contains a completely valid string,
// IsInputValid is called.
bool CFilterEdit::Match (const TCHAR *pszText)
{
	bool bSuccess = true;

#ifdef FILTER_EDIT_BOOST
	if(m_pRegEx)
	{
		boost::match_results<const TCHAR *> what;
		bSuccess = boost::regex_match (pszText, what, *m_pRegEx, boost::match_default |	boost::match_partial);
	}
#else
	bSuccess = m_pRegEx->Match (pszText);

	if (!bSuccess) bSuccess = !*pszText;
#endif

	return bSuccess;
}

bool CFilterEdit::CanDelete ()
{
	CString strText;
	int iStartIndex = -1;
	int iEndIndex = -1;

	GetWindowText (strText);
	GetSel (iStartIndex, iEndIndex);

	if (iStartIndex == iEndIndex)
	{
		iEndIndex++;
	}

	strText.Delete (iStartIndex, iEndIndex - iStartIndex);
	return Match (strText) || (strText == "");
}

bool CFilterEdit::CanPaste ()
{
	bool bSuccess = OpenClipboard () != 0;

	if (bSuccess)
	{
#ifdef _UNICODE
		HANDLE hClipMem = ::GetClipboardData (CF_UNICODETEXT);
#else
		HANDLE hClipMem = ::GetClipboardData (CF_TEXT);
#endif

		bSuccess = hClipMem != 0;

		if (bSuccess)
		{
			const TCHAR *lpClipMem = (const TCHAR *) ::GlobalLock (hClipMem);

			bSuccess = lpClipMem != 0;

			if (bSuccess)
			{
				CString strClipText;
				CString strText;
				int iStartIndex = -1;
				int iEndIndex = -1;

				strClipText = lpClipMem;
				::GlobalUnlock (hClipMem);
				GetWindowText (strText);
				GetSel (iStartIndex, iEndIndex);
				strText.Delete (iStartIndex, iEndIndex - iStartIndex);
				strText.Insert (iStartIndex, strClipText);

				if (strText.GetLength () > static_cast<int> (GetLimitText ()))
				{
					strText = strText.Left (GetLimitText ());
				}

				bSuccess = Match (strText);
			}
		}

		::CloseClipboard ();
	}

	return bSuccess;
}

// Set control state to input OK and hide tooltip
void CFilterEdit::ResetValid ()
{
	if (!m_bInputValid)
	{
		SetInputValid (true);
		DisplayToolTip (FALSE);
	}
}

// Set control state to input OK/not OK, update the visuals and
// set the focus if the input is not valid.
void CFilterEdit::SetInputValid (const bool bInputValid)
{
	m_bInputValid = bInputValid;
	Invalidate ();

	if (!m_bInputValid) SetFocus ();
}

void CFilterEdit::DisplayToolTip (const bool bDisplay)
{
	if (m_ToolTip.m_hWnd)
	{
		CToolInfo ti;

		m_ToolTip.GetToolInfo (ti, this, 0);

		if (bDisplay)
		{
			RECT ClientRect;
			int iHeight = 0;
			int iOffset = 0;

			GetRect (&ClientRect);
			iHeight = ClientRect.bottom - ClientRect.top;
			iOffset = (ClientRect.right - ClientRect.left) / 2;
			::GetWindowRect (m_hWnd, &ti.rect);
			ti.rect.top += iHeight;
			ti.rect.bottom += iHeight;
			ti.rect.left += iOffset;
			ti.rect.right += iOffset;

			m_ToolTip.SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (ti.rect.left,
				ti.rect.top));
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, TRUE,
				reinterpret_cast<LPARAM> (&ti));
		}
		else
		{
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,
				reinterpret_cast<LPARAM> (&ti));
		}
	}
}

// private functions:

void CFilterEdit::SetDefaults ()
{
	m_bInputValid = true;
	m_bControlDown = false;
	m_bSetBkOnError = false;
	m_bWavyLineOnError = true;
	m_crFgFocus = RGB (0, 0, 0);
	m_crBkFocus = RGB (255, 255 , 255);
	m_crFgOK = RGB (0, 0, 0);
	m_crBkOK = RGB (255, 255 , 255);
	m_crFgError = RGB (255, 255, 255);
	m_crBkError = RGB (255, 0 , 0);
}

void CFilterEdit::ConstructRegEx ()
{
	if (m_pRegEx == 0)
	{
		m_pRegEx = new CRegEx;

		if (m_pRegEx == 0)
		{
			AfxThrowMemoryException ();
		}

		m_bRegExOwner = true;
	}
}

BEGIN_MESSAGE_MAP(CFilterEdit, CStdEdit)
	//{{AFX_MSG_MAP(CFilterEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterEdit message handlers

// Intercept messages that change the text and validate them.
LRESULT CFilterEdit::WindowProc (UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	bool bExecute = true;
	bool bResetValid = false;

	if (m_bProcessChars)
	{
		switch (message)
		{
			case EM_REPLACESEL:
			{
				int iStartIndex = -1;
				int iEndIndex = -1;
				CString strReplace;

				GetSel (iStartIndex, iEndIndex);
				strReplace = reinterpret_cast<LPCTSTR> (lParam);
				GetWindowText (m_strProposedText);
				m_strProposedText.Delete (iStartIndex, iEndIndex -
					iStartIndex);
				m_strProposedText.Insert (iStartIndex, strReplace);
				bExecute = bResetValid = Match (m_strProposedText);
				break;
			}
			case WM_CLEAR:
			case WM_CUT:
				bExecute = bResetValid = CanDelete ();
				break;
			case WM_PASTE:
				bExecute = bResetValid = CanPaste ();
				break;
			case WM_SETTEXT:
			{
				const TCHAR *psz = reinterpret_cast<const TCHAR *>
					(lParam);

				bExecute = bResetValid = OnSetText(psz);
				break;
			}
			default:
				break;
		}
	}
	else
	{
		bResetValid = true;
	}

	if (bExecute)
	{
		lResult = CStdEdit::WindowProc (message, wParam, lParam);

		if (bResetValid) ResetValid ();
	}

	return lResult;
}

bool CFilterEdit::OnSetText(const TCHAR *szText)
{
	return IsTextValid (szText);
}

void CFilterEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_bProcessChars || ValidateChar (nChar))
	{
		CStdEdit::OnChar (nChar, nRepCnt, nFlags);
	}
}

void CFilterEdit::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bExecute = true;

	ResetValid ();

	if (nChar == VK_CONTROL)
	{
		m_bControlDown = true;
	}
	// VK_DELETE never reaches OnChar, so we process it here.
	else if (nChar == VK_DELETE)
	{
		if (m_bProcessChars)
		{
			bExecute = CanDelete ();
		}
	}
	// ^X, ^C and ^V do not have virtual key codes so we cannot
	// cleanly process them in OnChar. CFilterEdit::WindowProc deals with
	// responding to WM_CUT, WM_COPY and WM_PASTE
	else if ((nChar == 'x' || nChar == 'X') && m_bControlDown)
	{
		SendMessage (WM_CUT, 0, 0);
		// Will handle cut ourselves, so don't pass char on...
		bExecute = false;
	}
	else if ((nChar == 'c' || nChar == 'C') && m_bControlDown)
	{
		SendMessage (WM_COPY, 0, 0);
		// Will handle copy ourselves, so don't pass char on...
		bExecute = false;
	}
	else if ((nChar == 'v' || nChar == 'V') && m_bControlDown)
	{
		SendMessage (WM_PASTE, 0, 0);
		// Will handle paste ourselves, so don't pass char on...
		bExecute = false;
	}

	if (bExecute)
	{
		CStdEdit::OnKeyDown (nChar, nRepCnt, nFlags);
	}
}

void CFilterEdit::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL)
	{
		m_bControlDown = false;
	}

	CStdEdit::OnKeyUp (nChar, nRepCnt, nFlags);
}

void CFilterEdit::OnKillFocus (CWnd *pNewWnd)
{
	// Pass in false so that focus is not set back
	// to this control (see function SetInputValid).
	m_bInputValid = IsInputValid ();
	Invalidate ();
	DisplayToolTip (FALSE);

	// Don't allow cursor to disappear...
	if (pNewWnd && pNewWnd->m_hWnd == m_ToolTip.m_hWnd) return;
	
	CStdEdit::OnKillFocus (pNewWnd);
	
}

void CFilterEdit::OnSetFocus (CWnd *pOldWnd)
{
	CStdEdit::OnSetFocus (pOldWnd);
	Invalidate ();

	if (!m_bInputValid) DisplayToolTip (true);
}

HBRUSH CFilterEdit::CtlColor (CDC *pDC, UINT nCtlColor)
{
	HBRUSH hbr = 0;

	if (!(GetStyle () & ES_READONLY) && IsWindowEnabled ())
	{
		if (m_bInputValid)
		{
			if (GetFocus () == this)
			{
				hbr = m_brFocus;
				pDC->SetBkColor (m_crBkFocus);
				pDC->SetTextColor (m_crFgFocus);
			}
			else
			{
				hbr = m_brOK;
				pDC->SetBkColor (m_crBkOK);
				pDC->SetTextColor (m_crFgOK);
			}
		}
		else if (m_bSetBkOnError)
		{
			// If m_bSetBkOnError is set, then change
			// all the colours.
			hbr = m_brError;
			pDC->SetBkColor (m_crBkError);
			pDC->SetTextColor (m_crFgError);
		}
	}

	return hbr;
}

void CFilterEdit::OnPaint ()
{
	if (GetStyle () & ES_READONLY)
	{
		CStdEdit::OnPaint ();
	}
	else
	{
		CClientDC dc (this);

		//  This stops the control from going grey
		Default ();

		if (!m_bInputValid && m_bWavyLineOnError)
		{
			CFont *pFont = GetFont ();
			CPen *pPen = dc.GetCurrentPen ();
			LOGPEN lp;
			CPen MyPen;
			CRect rect;
			CString strText;
			CSize size;
			int iX = 0;
			const int iIncrement = 2;

			// We must use the correct font when using GetTextExtent..!
			dc.SelectObject (pFont);
			// We have to create a new pen to set a new colour...
			pPen->GetLogPen (&lp);
			lp.lopnColor = m_bSetBkOnError ? m_crFgError : m_crBkError;
			MyPen.CreatePenIndirect (&lp);
			dc.SelectObject (&MyPen);
			// Get the rect for the entire edit control
			GetRect (&rect);
			GetWindowText (strText);
			// This part deals with the *displayed* Text Extent
			size = dc.GetTextExtent (strText.
				Mid (CharFromPos (CPoint (rect.left + 1, rect.top))));

			// Check for case of empty string
			if (size.cx == 0) size.cx = iIncrement * 3;
			// Dont draw off the end of the edit control
			else if (size.cx > rect.right) size.cx = rect.right;

			// Starting x co-ordinate to start drawing from
			iX = rect.left;
			// Start at the bottom left of the edit control
			dc.MoveTo (rect.left, rect.bottom);

			// Draw the wavy line like in Microsoft Word
			while (iX < size.cx + rect.left)
			{
				iX += iIncrement;

				if (iX > size.cx + rect.left) break;

				dc.LineTo (iX, rect.bottom + iIncrement);
				iX += iIncrement;

				if (iX > size.cx + rect.left) break;

				dc.LineTo (iX, rect.bottom);
			}
		}
	}
}
