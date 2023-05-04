#if !defined(DATERANGECTRL_H)
#define DATERANGECTRL_H


#include <types/DateRange.h>

class CDateRangeCtrl : public CStatic
{

public:
	CDateRangeCtrl();
	virtual ~CDateRangeCtrl();

	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);	
	
	DateRange GetRange() const;
	void SetRange(const DateRange& range);
private:
		
	CDateTimeCtrl *m_pDateFrom;
	CStatic *m_pSeparator;
	CDateTimeCtrl *m_pDateTo;

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
