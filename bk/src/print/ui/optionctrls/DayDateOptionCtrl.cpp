#include "stdafx.h"
#include "DayDateOptionCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../options/PrintOptionSerializer.h"

#define ID_EDIT 1001

BEGIN_MESSAGE_MAP(DayDateOptionCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

DayDateOptionCtrl::DayDateOptionCtrl()
{	
}

DayDateOptionCtrl::~DayDateOptionCtrl()
{
}

PrintOption DayDateOptionCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer serializer;
	serializer.SetValue(option, GetDate());	
	return option;
}

void DayDateOptionCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer serializer;
	SetDate(serializer.GetDateTimeValue(option));
}


void DayDateOptionCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void DayDateOptionCtrl::CreateContent()
{
	m_edit.Create(WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, ID_EDIT);
	m_edit.SetFont(GetFont());
	SetWindowPos(NULL, 0, 0, 0, 21, SWP_NOMOVE);
	
}

void DayDateOptionCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
}

DateTime DayDateOptionCtrl::GetDate() const
{
	if (::IsWindow(m_edit.m_hWnd))
		return m_edit.GetDate();

	return DateTime();
}

void DayDateOptionCtrl::SetDate(const DateTime& date)
{
	if (::IsWindow(m_edit.m_hWnd))
		m_edit.SetDate(date);
}


void DayDateOptionCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);		
	}
}