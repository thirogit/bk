#include "stdafx.h"
#include "HerdNumbersBar.h"
#include <string\TextUtils.h>
#include "../../resource.h"
#include "../../utils/StringLoader.h"

CHerdNumbersBar::CHerdNumbersBar(uint32_t herdId) : m_herdId(herdId)
{

}

CHerdNumbersBar::~CHerdNumbersBar()
{
}


BEGIN_MESSAGE_MAP(CHerdNumbersBar, CSessionAwareWnd<CXColorStatic>)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL CHerdNumbersBar::Create(const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	return CSessionAwareWnd<CXColorStatic>::Create(L"", WS_CHILD | SS_RIGHT | SS_CENTERIMAGE | WS_VISIBLE, rect, pParentWnd,nID);
}

void CHerdNumbersBar::OnDestroy()
{
	GetSeasonSession()->RemoveHerdObserver(m_herdId,this);
	CSessionAwareWnd<CXColorStatic>::OnDestroy();
}

int CHerdNumbersBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CSessionAwareWnd<CXColorStatic>::OnCreate(lpCreateStruct) != 0)
		return -1;	

	SeasonSession* session = GetSeasonSession();
	session->AddHerdObserver(m_herdId, this);
	SetBackgroundColor(RGB(0,122,204), FALSE);
	SetTextColor(RGB(255,255,255), FALSE);
	SetPadding(20,20, 0,0);

	UpdateNumbers();
	return 0;
}

void CHerdNumbersBar::UpdateNumbers()
{
	SeasonSession* session = GetSeasonSession();
	HerdRegistry* registry = session->GetHerdRegistry(m_herdId);
	size_t allCows = registry->GetCountOfAllCows();
	size_t inStockCows = registry->GetCountOfInStockCows();

	SetWindowText(TextUtils::Format(StringLoader(IDS_HERDNUMBERS), allCows, inStockCows).c_str());
}

void CHerdNumbersBar::OnHerdCowAdded(const CowPresenceKey& key)
{
	UpdateNumbers();
}

void CHerdNumbersBar::OnHerdCowRemoved(const CowPresenceKey& key)
{
	UpdateNumbers();
}

void CHerdNumbersBar::OnHerdCowDeleted(const CowPresenceKey& key)
{
	UpdateNumbers();
}

void CHerdNumbersBar::OnHerdCowMoved(const CowPresenceKey& fromKey, const CowPresenceKey& toKey)
{
	UpdateNumbers();
}

void CHerdNumbersBar::OnHerdCowRevoked(const CowPresenceKey& fromKey, const CowPresenceKey& toKey)
{
	UpdateNumbers();
}
