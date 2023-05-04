#if !defined(DECIMALRANGECTRL_H)
#define DECIMALRANGECTRL_H

#include <types/DecimalRange.h>
#include "../decimaledit/DecimalEdit.h"

class CDecimalRangeCtrl : public CStatic
{

public:
	CDecimalRangeCtrl(int precision = 6);
	virtual ~CDecimalRangeCtrl();

	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);	
	
	DecimalRange GetRange() const;
private:
		
	CDecimalEdit *m_pFrom;
	CStatic *m_pSeparator;
	CDecimalEdit *m_pTo;
	int m_precision;

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
