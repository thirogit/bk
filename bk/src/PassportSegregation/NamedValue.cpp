#include "stdafx.h"
#include "NamedValue.h"


CNamedValue::CNamedValue() : COwnClassWnd(L"__named_value__")
{
}

void CNamedValue::Create(const std::wstring& sName,const std::wstring& sValue,const CRect& rect,CWnd* pParent,UINT nID)
{
	if(COwnClassWnd::Create(L"",WS_VISIBLE|WS_CHILD,rect,pParent,nID))
	{
		CRect clientRect;
		GetClientRect(&clientRect);

		int h = clientRect.Height();
		int halfH = h/2;
		int headerH = h-halfH;
		int valueH = h - headerH;

		m_header.Create(sName.c_str(),WS_VISIBLE|WS_CHILD|SS_CENTERIMAGE|SS_CENTER ,CRect(0,0,clientRect.right,headerH),this);
		m_value.Create(sValue.c_str(),WS_VISIBLE|WS_CHILD|SS_CENTERIMAGE|SS_CENTER ,CRect(0,headerH,clientRect.right,headerH+valueH),this);

		m_header.SetBackgroundColor(RGB(255,255,255),FALSE);
		m_value.SetBackgroundColor(RGB(255,255,255),FALSE);		
	}	
}

void CNamedValue::SetValue(const std::wstring& sValue)
{
	if(::IsWindow(m_value.m_hWnd))
		m_value.SetWindowText(sValue.c_str());
}

void CNamedValue::SetHeaderColor(COLORREF clr)
{
	m_header.SetBackgroundColor(clr);
}

void CNamedValue::SetValueColor(COLORREF clr)
{
	m_value.SetBackgroundColor(clr);
}