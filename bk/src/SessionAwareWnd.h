#pragma once

#include <afxcmn.h>
#include "context\SeasonSession.h"
#include "SeasonAccess.h"

#define WM_GETSEASONSESSION (WM_APP+10001)

template<class WND>
class CSessionAwareWnd : public WND
{
public:

	CSessionAwareWnd()
	{

	}

	template<typename A1,typename A2,typename A3>
	CSessionAwareWnd(A1 a1, A2 a2, A3 a3) : WND(a1, a2, a3)
	{

	}

	SeasonSession* GetSeasonSession()
	{
		return (SeasonSession*)SendMessage(WM_GETSEASONSESSION);
	}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(message == WM_GETSEASONSESSION)
		{
			CWnd* pParentWnd = GetParent();
			SeasonSession* pSession =  (SeasonSession*)pParentWnd->SendMessage(WM_GETSEASONSESSION);
			if(pSession == NULL)
			{
				pSession = SeasonAccess::GetAssociatedSeason(pParentWnd);
			}

			return (LRESULT)pSession;
		}
		
		return WND::WindowProc(message, wParam, lParam);
	}

};
