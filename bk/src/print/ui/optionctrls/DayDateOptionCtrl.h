#ifndef __DAYDATEOPTIONCTRL_H__
#define __DAYDATEOPTIONCTRL_H__

#include "DocumentOptionCtrl.h"
#include "../../../ctrls/dateedit/dateedit.h"

class DayDateOptionCtrl : public DocumentOptionCtrl
{	
public:
	DayDateOptionCtrl();
	virtual ~DayDateOptionCtrl();

	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	DateTime GetDate() const;
	void SetDate(const DateTime& date);
protected:
	virtual void CreateContent();	
private:
			
	CDateEdit m_edit;
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

