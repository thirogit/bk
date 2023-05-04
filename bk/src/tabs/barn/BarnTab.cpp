#include "stdafx.h"
#include "BarnTab.h"
#include "../../SeasonAccess.h"
#include "../../ContextAccess.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HERDS_TAB_WIDTH 20

#define ID_HERDS_TABCTRL_ID 1001

#define ID_HERDTAB_ID_MIN 2000

BarnTab::BarnTab()
{



}

BarnTab::~BarnTab()
{
	DeepDelete(m_HerdTabs);
}


BEGIN_MESSAGE_MAP(BarnTab, TabPage)
	ON_WM_CREATE()
	ON_NOTIFY(CTCN_TABSELECTED, ID_HERDS_TABCTRL_ID, OnHerdTabSelected)
	ON_NOTIFY(CTCN_TABUNSELECTED, ID_HERDS_TABCTRL_ID, OnHerdTabUnselected)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void BarnTab::OnDestroy()
{
	TabPage::OnDestroy();	
}

void BarnTab::OnSize( UINT nType,  int cx,  int cy  )
{
	TabPage::OnSize(nType,cx,cy);

	m_HerdsTab.SetWindowPos(NULL,0,0,HERDS_TAB_WIDTH,cy,SWP_NOZORDER | SWP_NOMOVE);

	std::for_each(m_HerdTabs.begin(),m_HerdTabs.end(),[cx,cy](HerdTab* tab)
														{
															tab->SetWindowPos(NULL,HERDS_TAB_WIDTH,0,cx-HERDS_TAB_WIDTH,cy,SWP_NOZORDER);
														});

}


BOOL BarnTab::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	return TabPage::Create( dwStyle, rect, pParentWnd, nID);
}


int BarnTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(TabPage::OnCreate(lpCreateStruct) != 0)
		return -1;
	

	if(!m_HerdsTab.Create(WS_CHILD| WS_VISIBLE | CTCS_LEFT,CRect(),this,ID_HERDS_TABCTRL_ID))
		return -1;

	SeasonSession* pSession = GetSeasonSession();
	XContext* pContext = ContextAccess::GetXContext();

	std::vector<IHerd*> herds;

	PtrEnumerator<IHerd> herdEnum = pContext->EnumHerds();
	while (herdEnum.hasNext())
	{
		herds.push_back(*herdEnum);
		herdEnum.advance();
	}

	std::sort(herds.begin(), herds.end(), [](IHerd* herd1, IHerd* herd2) -> bool 
	{
		return herd1->GetHerdIndex() < herd2->GetHerdIndex();
	});

	int item = 0;
	BOOST_FOREACH(IHerd* pHerd, herds)
	{
		
		HerdTab* tab = new HerdTab(pHerd);
		m_HerdTabs.push_back(tab);
		tab->Create(WS_CHILD ,CRect(0,0,100,100),this,ID_HERDTAB_ID_MIN+item);		
		m_HerdsTab.InsertItem(item,pHerd->GetHerdName().c_str(),(LPARAM)tab);
		
		item++;
	}

	m_HerdsTab.SetCurSel(0,TRUE);
	
	return 0;
}

void BarnTab::OnHerdTabUnselected(NMHDR* pNMHDR, LRESULT* pResult)
{
	CTC_NMHDR* pTabNHdr = (CTC_NMHDR*)pNMHDR;
	HerdTab* tab = (HerdTab*)(pTabNHdr->lParam);
	tab->ShowWindow(SW_HIDE);
	*pResult = 0;
}

void BarnTab::OnHerdTabSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTC_NMHDR* pTabNHdr = (CTC_NMHDR*)pNMHDR;
	HerdTab* tab = (HerdTab*)(pTabNHdr->lParam);
	tab->ShowWindow(SW_SHOW);
  

    *pResult = 0;
}