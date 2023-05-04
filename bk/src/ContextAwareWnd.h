#pragma once

#include <afxcmn.h>
#include "context\SeasonSession.h"
#include "SeasonAccess.h"

#define WM_GETXCONTEXT (WM_APP+10002)


template<class WND>
class CXContextAwareWnd : public WND
{
public:
	XContext* GetXContext()
	{
		return (XContext*)SendMessage(WM_GETXCONTEXT);
	}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(message == WM_GETXCONTEXT)
		{
			CWnd* pParentWnd = GetParent();
			XContext* context =  (SeasonSession*)pParentWnd->SendMessage(WM_GETXCONTEXT);
			if(pSession == NULL)
			{
				pSession = SeasonAccess::GetAssociatedSeason(pParentWnd);
			}

			return (LRESULT)pSession;
		}
		
		return WND::WindowProc(message, wParam, lParam);
	}

};
