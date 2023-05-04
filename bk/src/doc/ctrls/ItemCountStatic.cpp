#include "stdafx.h"
#include "ItemCountStatic.h"
#include <string\TextUtils.h>

BEGIN_MESSAGE_MAP(CItemCountStatic,CStatic)

	ON_WM_CREATE()

END_MESSAGE_MAP()


CItemCountStatic::CItemCountStatic(DocTransactionFacade* pFacade) : m_pFacade(pFacade)
{
}

void CItemCountStatic::SetItemCount(size_t count)
{
	SetWindowText(TextUtils::FormatA(L"%d",count).c_str());
}

void CItemCountStatic::UpdateCount()
{
	SetItemCount(m_pFacade->GetItemCount());
}

void CItemCountStatic::OnItemUpdated(ItemId id)
{
	UpdateCount();
}

void CItemCountStatic::OnItemAdded(ItemId id)
{
	UpdateCount();
}

void CItemCountStatic::OnItemDeleted(ItemId id)
{
	UpdateCount();
}

void CItemCountStatic::Register()
{
	m_pFacade->AddObserver(this);
	UpdateCount();
}

void CItemCountStatic::Unregister()
{
	m_pFacade->RemoveObserver(this);
}


void CItemCountStatic::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	Register();
}

int CItemCountStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CStatic::OnCreate(lpCreateStruct) == -1) return -1;
	Register();
	return 0;
}

void CItemCountStatic::OnDestroy()
{
	CStatic::OnDestroy();
	Unregister();
}
