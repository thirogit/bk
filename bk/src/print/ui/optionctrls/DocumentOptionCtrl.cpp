#include "stdafx.h"
#include "DocumentOptionCtrl.h"


void DocumentOptionCtrl::Create(const std::wstring& sOptionName,CWnd* pParent)
{
	CStatic::Create(L"", WS_CHILD , CRect(0, 0, 0, 0), pParent, ObtainId(pParent));
	//ModifyStyleEx(GetExStyle(), WS_EX_CONTROLPARENT);
	SetFont(pParent->GetFont());
	m_optionName = sOptionName;
	CreateContent();
}

UINT DocumentOptionCtrl::ObtainId(CWnd* pWnd)
{
	UINT id = 0;

	CWnd* pChild = pWnd->GetWindow(GW_HWNDFIRST);
	while (pChild != NULL)
	{
		id = max(id, pChild->GetDlgCtrlID());
		pChild = pWnd->GetNextWindow(GW_HWNDNEXT);
	}

	return id+1000;
}

const std::wstring& DocumentOptionCtrl::GetOptionCode() const
{
	return m_optionName;
}


