#ifndef __TIPEDITWRAPPER_H__
#define __TIPEDITWRAPPER_H__

template<class EDIT>
class CTipEditWrapper : public EDIT
{
public :
	CTipEditWrapper() : m_hAbortWnd(NULL) {}

	void SetAbortWnd(CWnd *pWnd)
	{
		m_hAbortWnd = pWnd->GetSafeHwnd();
	}
	
protected:	
	virtual void PreSubclassWindow()
	{
		EDIT::PreSubclassWindow();
	}
	
	// Tooltip functions	
	/*
	TTI_NONE No icon.
	TTI_INFO Info icon.
	TTI_WARNING Warning icon
	TTI_ERROR Error Icon
	TTI_INFO_LARGE Large error Icon
	TTI_WARNING_LARGE Large error Icon
	TTI_ERROR_LARGE Large error Icon
	*/

	void CreateToolTip ()
	{
		if (m_ToolTip.m_hWnd) return;

		TOOLINFO ti;
		DWORD dwStyle = TTS_BALLOON;
		m_ToolTip.Create (GetParent(), dwStyle);
		m_ToolTip.FillInToolInfo (ti, this, 0);
		ti.uFlags |= TTF_TRACK | TTF_CENTERTIP | TTF_ABSOLUTE;
		ti.lpszText = L"";
		//m_ToolTip.SetTitle (iIconType, pszTitle);
		m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM> (&ti));
	}

	void SetToolTipText (const TCHAR *pszText)
	{
		CreateToolTip ();
		m_ToolTip.UpdateTipText (pszText, this);
	}

	void SetToolTipTitle (const int iIconType, const TCHAR *pszTitle)
	{
		CreateToolTip ();
		m_ToolTip.SetTitle (iIconType, pszTitle);
	}

	void ShowToolTip()
	{
		CreateToolTip ();
		if (m_ToolTip.m_hWnd)
		{
			CToolInfo ti;

			m_ToolTip.GetToolInfo (ti, this, 0);

			RECT ClientRect;
			int iHeight = 0;
			int iOffset = 0;

			GetRect (&ClientRect);
			iHeight = ClientRect.bottom - ClientRect.top;
			iOffset = (ClientRect.right - ClientRect.left) / 2;
			::GetWindowRect (m_hWnd, &ti.rect);
			ti.rect.top += iHeight;
			ti.rect.bottom += iHeight;
			ti.rect.left += iOffset;
			ti.rect.right += iOffset;

			m_ToolTip.SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (ti.rect.left, ti.rect.top));
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, TRUE,	reinterpret_cast<LPARAM> (&ti));			
		}
	}

	void HideToolTip()
	{
		if (m_ToolTip.m_hWnd)
		{
			CToolInfo ti;
			m_ToolTip.GetToolInfo (ti, this, 0);
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,	reinterpret_cast<LPARAM> (&ti));
		}
	}

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
			case WM_CONTEXTMENU:
				HideToolTip();				
				break;
			case WM_KILLFOCUS:
				HideToolTip ();
				// Don't allow cursor to disappear...
				if ((wParam && ((HWND)wParam) != m_ToolTip.m_hWnd))
				{
					if(m_hAbortWnd && ((HWND)wParam) == m_hAbortWnd) 
						return (LRESULT)0;					
				}
				break;
			case WM_KEYDOWN:
				HideToolTip();
				break;
		}		
		
		return EDIT::WindowProc(message, wParam, lParam);
	}

	CToolTipCtrl m_ToolTip;
	HWND m_hAbortWnd;
};


#endif