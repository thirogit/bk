#ifndef __COLOREDIT_H__
#define __COLOREDIT_H__

#include "../../uitypes/Color.h"

template<class BASE>
class CColorEdit : public BASE
{
	struct CTLCOLOR
	{
		HWND hWnd;
		HDC hDC;
		UINT nCtlType;
	};
public:
	CColorEdit() 
	{
	}

	~CColorEdit()
	{
		DeleteBackgroundBrush();
	}

protected:
	
	
	void DeleteBackgroundBrush()
	{
		if (m_brBackGnd.GetSafeHandle())
		   m_brBackGnd.DeleteObject();
	}
		

	BOOL OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
		{					
			BOOL bResult = TRUE;
			CDC dcTemp; 
			dcTemp.m_hDC = (HDC)wParam;
			UINT nCtlType = uMsg - WM_CTLCOLORMSGBOX;
			HBRUSH hbr = CtlColor(&dcTemp, nCtlType);
			dcTemp.m_hDC = NULL;
			*pResult = reinterpret_cast<LRESULT>(hbr);
			
			if ((HBRUSH)*pResult == NULL) bResult = FALSE;
			return bResult;
		}
		else
		{
			return BASE::OnChildNotify(uMsg, wParam, lParam, pResult);
		}
	}

	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor) 
	{
		if(m_brBackGnd.GetSafeHandle())
		{
			LOGBRUSH logBrush;
			m_brBackGnd.GetLogBrush(&logBrush);
			pDC->SetBkColor(logBrush.lbColor);
			return (HBRUSH)m_brBackGnd.GetSafeHandle();
		}
		return (HBRUSH)NULL;
	}
public:
	void SetBkgColor(const Color& bkgColor)
	{
		DeleteBackgroundBrush();
		if(!bkgColor.IsNull())
			m_brBackGnd.CreateSolidBrush(bkgColor);

		Invalidate(TRUE);
	}
		

private:	
	CBrush m_brBackGnd;	
	
};

#endif 
