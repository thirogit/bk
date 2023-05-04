#ifndef __BOOLEANOPTIONCTRL_H__
#define __BOOLEANOPTIONCTRL_H__

#include "DocumentOptionCtrl.h"
#include <vector>
#include <types\Decimal.h>
#include <afxcmn.h>

class BooleanOptionCtrl : public DocumentOptionCtrl
{	
public:
	BooleanOptionCtrl();
	~BooleanOptionCtrl();

	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	bool GetBoolean() const;
	void SetBoolean(bool b);
protected:
	virtual void CreateContent();

	void CheckButtons();
private:	
	bool m_Value;
	CToolBarCtrl m_YNBtns;
protected:
	afx_msg void OnDestroy();
	afx_msg void OnY();
	afx_msg void OnN();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

