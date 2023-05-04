#if !defined(AGERANGECTRL_H)
#define AGERANGECTRL_H

#include <types/IntRange.h>
#include "../integeredit/IntegerEdit.h"
#include "../dwmbtn/DayWeekMonthBtn.h"

class CAgeRangeCtrl : public CStatic
{

public:
	CAgeRangeCtrl();
	virtual ~CAgeRangeCtrl();

	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);	
	
	IntRange GetRange() const;
	CalendarUnit GetUnit() const;
private:
		
	CIntegerEdit *m_pFrom;
	CStatic *m_pSeparator;
	CIntegerEdit *m_pTo;
	CDayWeekMonthBtn* m_pUnit;

protected:
	BOOL InitControls();
	void PosControls(void);
	virtual void PreSubclassWindow();


	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnable(BOOL bEnable); 
	
	
	DECLARE_MESSAGE_MAP()
};

#endif
