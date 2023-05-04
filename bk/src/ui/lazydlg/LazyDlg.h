#pragma once

#include <afxcmn.h>

#define WM_AFTERDLGSHOW (WM_APP+10002)

template<class DLG>
class CLazyDlg : public DLG
{
public:
	CLazyDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL) : DLG(nIDTemplate,pParentWnd)
	{
	}
protected:
	virtual void OnAfterDlgShow() = 0;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
			case  WM_INITDIALOG:
				m_dlgshown = false;
				break;
				
			case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS* windowpos = (WINDOWPOS*)lParam;
				if((windowpos->flags & SWP_SHOWWINDOW) && !m_dlgshown)
				{
					m_dlgshown = true;
					PostMessage(WM_AFTERDLGSHOW);
				}
				break;
			}
			
			case WM_AFTERDLGSHOW:
			{
				OnAfterDlgShow();
				return (LRESULT)0;
			}
		}
		
		return DLG::WindowProc(message, wParam, lParam);
	}
private:
	bool m_dlgshown;

};
