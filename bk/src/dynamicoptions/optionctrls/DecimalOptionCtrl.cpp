#include "stdafx.h"
#include "DecimalOptionCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../options/PrintOptionSerializer.h"

#define ID_EDIT 1001

BEGIN_MESSAGE_MAP(DecimalOptionCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

DecimalOptionCtrl::DecimalOptionCtrl() : m_edit(6)
{	
}

DecimalOptionCtrl::~DecimalOptionCtrl()
{
}

PrintOption DecimalOptionCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer serializer;
	serializer.SetValue(option, GetDecimal());
	return option;
}

void DecimalOptionCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer serializer;
	SetDecimal(serializer.GetDecimalValue(option));
}


void DecimalOptionCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void DecimalOptionCtrl::CreateContent()
{
	m_edit.Create(WS_BORDER | WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1001);
	m_edit.SetFont(GetFont());
	SetWindowPos(NULL, 0, 0, 0, 21, SWP_NOMOVE);
}

void DecimalOptionCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
}

NullDecimal DecimalOptionCtrl::GetDecimal() const
{
	NullDecimal result;
	
	if (::IsWindow(m_edit.m_hWnd))
	{
		result = m_edit.GetDecimal();
	}
	return result;
}

void DecimalOptionCtrl::SetDecimal(const NullDecimal& decimal)
{
	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetDecimal(decimal);
	}		
}


void DecimalOptionCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	}
}