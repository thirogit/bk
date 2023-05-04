#include "stdafx.h"
#include "TimerTipWnd.h"

#define HIDE_TIMER_ID 1002
#define MIN_SHOW_TIME_MS 1000

BEGIN_MESSAGE_MAP(CTimerTipWnd,CTipWnd)
	
	ON_WM_TIMER()

END_MESSAGE_MAP()


CTimerTipWnd::CTimerTipWnd() 
{
}

CTimerTipWnd::~CTimerTipWnd()
{
}

void CTimerTipWnd::Show(UINT hideAfterMiliseconds)
{
	CTipWnd::Show();
	SetTimer(HIDE_TIMER_ID, max(hideAfterMiliseconds, MIN_SHOW_TIME_MS), NULL);
}

void CTimerTipWnd::OnTimer(UINT nTimerID)
{
	if(nTimerID == HIDE_TIMER_ID)
	{
		KillTimer(nTimerID);
		Hide();
	}
	CTipWnd::OnTimer(nTimerID);
}

