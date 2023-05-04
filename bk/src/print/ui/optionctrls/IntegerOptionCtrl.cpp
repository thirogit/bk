#include "stdafx.h"
#include "IntegerOptionCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../options/PrintOptionSerializer.h"

#define ID_EDIT 1001

BEGIN_MESSAGE_MAP(IntegerOptionCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

IntegerOptionCtrl::IntegerOptionCtrl()
{	
}

IntegerOptionCtrl::~IntegerOptionCtrl()
{
}

PrintOption IntegerOptionCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer serializer;
	serializer.SetValue(option, GetInteger());
	return option;
}

void IntegerOptionCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer serializer;
	SetInteger(serializer.GetIntegerValue(option));
}


void IntegerOptionCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void IntegerOptionCtrl::CreateContent()
{
	m_edit.Create(WS_BORDER | WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1001);
	m_edit.SetRegEx(L"[-+]?[0-9]{1,6}");
	m_edit.SetFont(GetFont());
	SetWindowPos(NULL, 0, 0, 0, 21, SWP_NOMOVE);
}

void IntegerOptionCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
}

NullInt IntegerOptionCtrl::GetInteger() const
{
	NullInt result;
	
	if (::IsWindow(m_edit.m_hWnd))
	{
		std::wstring sInteger = m_edit.GetText();
		if (!sInteger.empty())
			result = _wtoi(sInteger.c_str());
	}
	return result;
}

void IntegerOptionCtrl::SetInteger(const NullInt& integer)
{
	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetText(integer.ToString());
	}		
}


void IntegerOptionCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	}
}