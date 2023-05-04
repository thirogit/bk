#include "stdafx.h"
#include "stdedit.h"

void DDX_StdText(CDataExchange* pDX, int nIDC, std::wstring& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		wchar_t* szWindowText = new wchar_t[nLen+1];
		::GetWindowText(hWndCtrl, szWindowText, nLen+1);
		value = szWindowText;
		delete szWindowText;
	}
	else
	{
		::SetWindowText(hWndCtrl, value.c_str());
	}
}

std::wstring CStdEdit::GetText() const
{
	std::wstring result;
	int textLen = GetWindowTextLength();
	if(textLen > 0)
	{
		wchar_t* text = new wchar_t[textLen+1];
		GetWindowText(text,textLen+1);
		result = text;
		delete text;
	}
	return result;
}

void CStdEdit::SetText(const std::wstring& sText)
{
	SetWindowText(sText.c_str());
}
