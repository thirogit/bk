#include "stdafx.h"
#include "StringOptionCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../options/PrintOptionSerializer.h"

#define ID_EDIT 1001

BEGIN_MESSAGE_MAP(StringOptionCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

StringOptionCtrl::StringOptionCtrl()
{	
}

StringOptionCtrl::~StringOptionCtrl()
{
}

PrintOption StringOptionCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer serializer;
	serializer.SetValue(option, GetString());	
	return option;
}

void StringOptionCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer serializer;
	SetString(serializer.GetStringValue(option));
}


void StringOptionCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void StringOptionCtrl::CreateContent()
{
	m_edit.Create(WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1001);
	m_edit.SetFont(GetFont());
	SetWindowPos(NULL, 0, 0, 0, 21, SWP_NOMOVE);
	
}

void StringOptionCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
}

std::wstring StringOptionCtrl::GetString() const
{
	if (::IsWindow(m_edit.m_hWnd))
		return m_edit.GetText();

	return L"";
}

void StringOptionCtrl::SetString(const std::wstring& str)
{
	if (::IsWindow(m_edit.m_hWnd))
		m_edit.SetText(str);	
}


void StringOptionCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);		
	}
}