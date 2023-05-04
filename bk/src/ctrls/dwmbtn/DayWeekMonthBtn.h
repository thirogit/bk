#ifndef __DAYWEEKMONTHBTN_H__
#define __DAYWEEKMONTHBTN_H__

#include <types/CalendarUnit.h>


class CDayWeekMonthBtn : public CStatic
{
public:
	CDayWeekMonthBtn();
	virtual ~CDayWeekMonthBtn();

	void SetUnit(CalendarUnit unit);
	CalendarUnit GetUnit() const;

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	
protected:
	virtual void PreSubclassWindow();
	void CreateButtons();
	void CheckButtons();
	void ResizeButtons();
	
	CalendarUnit m_unit;
	CToolBarCtrl m_Btns;

	afx_msg void OnDay();
	afx_msg void OnWeek();
	afx_msg void OnMonth();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);


	DECLARE_MESSAGE_MAP()
};


#endif

