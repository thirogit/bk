#include "stdafx.h"
#include "HentNoOptionCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../options/PrintOptionSerializer.h"

#define ID_EDIT 1001

BEGIN_MESSAGE_MAP(HentNoOptionCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

HentNoOptionCtrl::HentNoOptionCtrl()
{	
}

HentNoOptionCtrl::~HentNoOptionCtrl()
{
}

PrintOption HentNoOptionCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer serializer;
	serializer.SetValue(option, GetHentNo().ToString());	
	return option;
}

void HentNoOptionCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer serializer;
	SetHentNo(HentNoFactory::FromString(serializer.GetStringValue(option)));
}


void HentNoOptionCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void HentNoOptionCtrl::CreateContent()
{
	m_edit.Create(WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1001);
	m_edit.SetFont(GetFont());
	SetWindowPos(NULL, 0, 0, 0, 21, SWP_NOMOVE);
	
}

void HentNoOptionCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
}

HentNo HentNoOptionCtrl::GetHentNo() const
{
	if (::IsWindow(m_edit.m_hWnd))
		return m_edit.GetHentNo();

	return HentNo();
}

void HentNoOptionCtrl::SetHentNo(const HentNo& hentNo)
{
	if (::IsWindow(m_edit.m_hWnd))
		m_edit.SetHentNo(hentNo);
}


void HentNoOptionCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	if (::IsWindow(m_edit.m_hWnd))
	{
		m_edit.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);		
	}
}