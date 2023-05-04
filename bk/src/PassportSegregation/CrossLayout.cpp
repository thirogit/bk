#include "stdafx.h"
#include "CrossLayout.h"

CrossLayout::CrossLayout(CPoint origin) : m_origin(origin)
{
	m_Fourths.fill(NULL);
}

bool CrossLayout::Add(CWnd* pWnd)
{
	for(int i = 0;i < 4;i++)
	{
		if(m_Fourths[i] == NULL)
		{
			m_Fourths[i] = pWnd;
			PositionFourths();			
			return true;
		}
	}
	return false;
}

void CrossLayout::PositionFourths()
{
	int cols = 0;
	int xOff = 0,yOff = 0;
	int i;
	for(i = 0;i < 4 && cols < 2;i++)
	{
		CWnd* pWnd = m_Fourths[i];
		if(pWnd)
		{
			CSize wndSize = GetWindowSize(pWnd);
			pWnd->SetWindowPos(NULL,m_origin.x+xOff,m_origin.y+yOff,0,0,SWP_NOSIZE|SWP_NOZORDER);
			cols++;

			xOff += wndSize.cx;
			yOff = max(yOff,wndSize.cy);
		}		
	}

	xOff = 0;
	for(;i < 4;i++)
	{
		CWnd* pWnd = m_Fourths[i];
		if(pWnd)
		{
			CSize wndSize = GetWindowSize(pWnd);
			pWnd->SetWindowPos(NULL,m_origin.x+xOff,m_origin.y+yOff,0,0,SWP_NOSIZE|SWP_NOZORDER);
			xOff += wndSize.cx;			
		}		
	}
	
}

CWnd* CrossLayout::GetFourth(CrossLayout::Fourth fourth) const
{
	if(fourth != Fourth_None)
	{
		return m_Fourths[fourth];
	}
	return NULL;
}

CSize CrossLayout::GetFourthSize(Fourth fourth) const
{
	CSize size(0,0);
	CWnd* pFourthWnd = GetFourth(fourth);
	if(pFourthWnd)
	{		
		size = GetWindowSize(pFourthWnd);
	}
	return size;
}

CSize CrossLayout::GetWindowSize(CWnd* pWnd) const
{
	CRect wndRect;
	pWnd->GetWindowRect(wndRect);
	return CSize(wndRect.Width(),wndRect.Height());
}

CSize CrossLayout::GetDesirableSize() const
{
	CSize sizeNW = GetFourthSize(Fourth_NorthWest);
	CSize sizeNE = GetFourthSize(Fourth_NorthEast);
	CSize sizeSW = GetFourthSize(Fourth_SouthWest);
	CSize sizeSE = GetFourthSize(Fourth_SouthWest);

	return CSize(max(sizeNW.cx + sizeNE.cx,sizeSW.cx + sizeSE.cx),max(sizeNW.cy + sizeSW.cy,sizeNE.cy + sizeSE.cy));
}


