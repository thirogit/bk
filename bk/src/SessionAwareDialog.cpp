#include "stdafx.h"
#include "SessionAwareDialog.h"
#include "SeasonAccess.h"

BEGIN_MESSAGE_MAP(CSessionAwareDialog, CDialog)	
	ON_MESSAGE(WM_GETSEASONSESSION, OnGetSeasonSession)
END_MESSAGE_MAP()



CSessionAwareDialog::CSessionAwareDialog()
{

}

CSessionAwareDialog::CSessionAwareDialog(UINT nIDTemplate, CWnd* pParentWnd) : CDialog(nIDTemplate,pParentWnd)
{
}

SeasonSession* CSessionAwareDialog::GetSeasonSession()
{
	SeasonSession* pSession = (SeasonSession*)m_pParentWnd->SendMessage(WM_GETSEASONSESSION);

	if(pSession == NULL)
		pSession = SeasonAccess::GetAssociatedSeason(m_pParentWnd);

	return pSession;
}

LRESULT CSessionAwareDialog::OnGetSeasonSession(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)GetSeasonSession();
}
