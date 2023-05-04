#include "stdafx.h"
#include "VerticalStackCtrl.h"
#include <boost\foreach.hpp>
#include <arrays\DeepDelete.h>

#define GAP 3

BEGIN_MESSAGE_MAP(VertialStackCtrl, CStatic)
	
	ON_WM_SIZE()	
	ON_WM_VSCROLL()
	ON_WM_DESTROY()

END_MESSAGE_MAP()



VertialStackCtrl::VertialStackCtrl() 
{

}


BOOL VertialStackCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CStatic::Create(L"", dwStyle, rect, pParentWnd, nID);
}

void VertialStackCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}

void VertialStackCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

}

void VertialStackCtrl::OnDestroy()
{
	CStatic::OnDestroy();

	BOOST_FOREACH(Control* control, m_controls)
	{
		if (control->header)
			delete control->header;
	}

	DeepDelete(m_controls);
}

void VertialStackCtrl::AddControl(CWnd* pCtrl)
{
	AddControl(NULL, pCtrl);
}

void VertialStackCtrl::AddControl(CXColorStatic *header, CWnd* pCtrl)
{
	CRect thisRect;
	GetClientRect(&thisRect);

	int H = ComputeHeight();
	int ctrlH = GetWindowH(pCtrl);

	Control* control = new Control();
	control->window = pCtrl;
	control->header = header;
	m_controls.push_back(control);

	if (header)
	{
		H += GAP;

		int headerH = GetWindowH(header);
		header->SetWindowPos(NULL, 0, H, thisRect.Width(), headerH, SWP_SHOWWINDOW);

		H += headerH;
	}

	H += GAP;

	pCtrl->SetParent(this);
	pCtrl->SetWindowPos(NULL, 0, H, thisRect.Width(), ctrlH, SWP_SHOWWINDOW);
}

int VertialStackCtrl::GetWindowH(CWnd* pWnd)
{
	int H = 0;
	if (pWnd)
	{
		CRect rect;
		pWnd->GetClientRect(&rect);
		H = rect.Height();
	}

	return H;
}

int VertialStackCtrl::ComputeHeight()
{
	int H = 0;
	BOOST_FOREACH(Control* control, m_controls)
	{
		if (control->header)
		{
			H += GAP;
			H += GetWindowH(control->header);
		}
		H += GAP;			
		H += GetWindowH(control->window);
	}
	return H;
}

void VertialStackCtrl::AddControl(const std::wstring& sHeaderText, CWnd* pCtrl)
{
	CXColorStatic* header = new CXColorStatic();
	
	header->Create(sHeaderText.c_str(), WS_CHILD | SS_LEFT | SS_CENTERIMAGE, CRect(0, 0, 0, 20),this);
	header->SetBackgroundColor(RGB(127, 127, 127));
	AddControl(header, pCtrl);
}
/*
PtrEnumerator<CWnd> VertialStackCtrl::EnumControls()
{
	return PtrEnumerator<CWnd>(m_controls);
}*/