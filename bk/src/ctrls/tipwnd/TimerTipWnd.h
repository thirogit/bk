#ifndef __TIMERTIPWND_H__
#define __TIMERTIPWND_H__

#include "TipWnd.h"

class CTimerTipWnd : public CTipWnd
{
public:
	CTimerTipWnd();	
	~CTimerTipWnd();
	
protected:
		
public:
	void Show( UINT hideAfterMiliseconds);	
	
protected:
	afx_msg void OnTimer(UINT nTimerID);

	DECLARE_MESSAGE_MAP();

};
#endif