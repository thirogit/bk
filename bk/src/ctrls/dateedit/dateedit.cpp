#include "stdafx.h"
#include "dateedit.h"
#include "../../resource.h"
#include "../../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CDateEdit,COXMaskedEdit)

	ON_WM_CREATE()

END_MESSAGE_MAP()


const wchar_t CDateEdit::m_DateMask[] = L"####-##-##";

CDateEdit::CDateEdit() : m_bDynCreate(false)
{

}
void CDateEdit::SetNowDate()
{
	SetDate(DateTime::now());
}

void CDateEdit::SetDate(const DateTime &date)
{
	if(date.IsNull()) 
		EmptyData(TRUE);
	else
		SetInputData(date.GetDateInFmt(DateTime::YYYYdashMMdashDD).c_str());	
}

void CDateEdit::PreSubclassWindow()
{
	COXMaskedEdit::PreSubclassWindow();
	if (!m_bDynCreate)
	{
		SetMask(m_DateMask);
	}
}

BOOL CDateEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_bDynCreate = true;
	return COXMaskedEdit::Create(dwStyle, rect, pParentWnd, nID);
}

int CDateEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(COXMaskedEdit::OnCreate(lpCreateStruct) == -1) return -1;
	SetMask(m_DateMask);
	return 0;
}

bool CDateEdit::IsDateValid()
{
	DateTime dt;
	std::wstring s;
	if(IsInputEmpty()) 
		return false;
	else	
		s = GetText();
		
	try
	{
		DateTime::ParseDate(DateTime::YYYYdashMMdashDD, s);
		return true;
	}
	catch (...)
	{
		return false;
	}	
}

DateTime CDateEdit::GetDate() const
{
	DateTime dt;

	if(!IsInputEmpty()) 
	{
		std::wstring s = GetText();
		dt = DateTime::ParseDate(DateTime::YYYYdashMMdashDD,s);	
	}
	return dt;
}

BOOL CDateEdit::OnValidate()
{
	if(!COXMaskedEdit::OnValidate()) return FALSE;

	if(!IsInputEmpty() && !IsDateValid())
	{
		SetToolTipText(StringLoader(IDS_ERRASKFORVALIDDATE).c_str());
		SetToolTipTitle(TTI_ERROR,StringLoader(IDS_ERRBADDATE).c_str());
		ShowToolTip();		
		return FALSE;
	}
	return TRUE;
}
