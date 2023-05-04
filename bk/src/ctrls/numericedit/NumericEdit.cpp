#include "stdafx.h"
#include "NumericEdit.h"
#include <boost\format.hpp>

//using namespace boost::xpressive;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNumericEdit::CNumericEdit (int precision) 
{
	m_bControlDown = false;

	std::wstring sRegEx = L"[0-9]{1,6}";
	if (precision > 0)
	{
		sRegEx += boost::str(boost::wformat(L"([.][0-9]{0,%d})?") % precision);
	}
	m_floatregex.assign(sRegEx);
}

CNumericEdit::~CNumericEdit ()
{
}

bool CNumericEdit::IsInputEmpty()
{
	return GetWindowTextLength() == 0;
}

bool CNumericEdit::IsValid (const TCHAR *pszText)
{
	bool bSuccess = true;

	boost::match_results<const TCHAR *> what;
	bSuccess = boost::regex_match(pszText, what, m_floatregex, boost::match_default);
	
	return bSuccess;
}

bool CNumericEdit::ValidateChar (UINT nChar)
{
	int iStartIndex = -1;
	int iEndIndex = -1;
	bool bCharValid = true;

	GetSel (iStartIndex, iEndIndex);
	CString text;
	GetWindowText (text);

	//  Backspace validation is done here as it has a virtual key code.
	if (nChar == VK_BACK)
	{
		if (iStartIndex == iEndIndex)
		{
			iStartIndex--;
		}

		// Remove char
		text.Delete (iStartIndex, iEndIndex - iStartIndex);
		if(text.IsEmpty()) bCharValid = true;
		else bCharValid = Match(text);
	}
	else
	{
		text.Delete(iStartIndex, iEndIndex - iStartIndex);
		text.Insert(iStartIndex, static_cast<TCHAR> (nChar));
		if (text.IsEmpty()) bCharValid = true;
		else bCharValid = Match(text);
	}

	return bCharValid;
}

// NOTE: This function checks if the given string matches SO FAR.
// To check if the edit control contains a completely valid string,
// IsInputValid is called.
bool CNumericEdit::Match (const TCHAR *pszText)
{
	bool bSuccess = true;

	bSuccess = IsValid(pszText);

	return bSuccess;
}

bool CNumericEdit::CanPaste ()
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



BEGIN_MESSAGE_MAP(CNumericEdit, CStdEdit)
	
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	
END_MESSAGE_MAP()

LRESULT CNumericEdit::WindowProc (UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	bool bExecute = true;

	switch (message)
	{
		case EM_REPLACESEL:
		{
			int iStartIndex = -1;
			int iEndIndex = -1;
			CString text,strReplace;

			GetSel (iStartIndex, iEndIndex);
			strReplace = reinterpret_cast<LPCTSTR> (lParam);
			GetWindowText(text);
			text.Delete(iStartIndex, iEndIndex - iStartIndex);
			text.Insert(iStartIndex, strReplace);
			bExecute = Match(text);
			break;
		}
		case WM_CLEAR:
		case WM_CUT:
			bExecute = true;
			break;
		case WM_PASTE:
			bExecute = CanPaste ();
			break;
		case WM_SETTEXT:
		{
			const TCHAR *psz = reinterpret_cast<const TCHAR *>(lParam);
			bExecute = OnSetText(psz);
			break;
		}
		default:
			break;
	}

	if (bExecute)
	{
		lResult = CStdEdit::WindowProc (message, wParam, lParam);
	}

	return lResult;
}

bool CNumericEdit::OnSetText(const TCHAR *szText)
{
	return IsValid (szText);
}

void CNumericEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == ',') nChar = '.';

	if (ValidateChar (nChar))
	{
		this->DefWindowProc(WM_CHAR, nChar, MAKELPARAM(nRepCnt, nFlags));			
		//CStdEdit::OnChar (nChar, nRepCnt, nFlags);
	}
}

void CNumericEdit::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bExecute = true;

	if (nChar == VK_CONTROL)
	{
		m_bControlDown = true;
	}
	// VK_DELETE never reaches OnChar, so we process it here.
	else if (nChar == VK_DELETE)
	{
		bExecute = true;
	}
	// ^X, ^C and ^V do not have virtual key codes so we cannot
	// cleanly process them in OnChar. CNumericEdit::WindowProc deals with
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

void CNumericEdit::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL)
	{
		m_bControlDown = false;
	}
	CStdEdit::OnKeyUp (nChar, nRepCnt, nFlags);
}

void CNumericEdit::OnKillFocus (CWnd *pNewWnd)
{
		CStdEdit::OnKillFocus (pNewWnd);
	
}

void CNumericEdit::OnSetFocus (CWnd *pOldWnd)
{
	CStdEdit::OnSetFocus (pOldWnd);
	
}

