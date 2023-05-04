#ifndef __INTEGEROPTIONCTRL_H__
#define __INTEGEROPTIONCTRL_H__

#include "DocumentOptionCtrl.h"
#include <types\NullInt.h>
#include "../../../ctrls/FilterEdit/FilterEdit.h"

class IntegerOptionCtrl : public DocumentOptionCtrl
{	
public:
	IntegerOptionCtrl();
	~IntegerOptionCtrl();

	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	NullInt GetInteger() const;
	void SetInteger(const NullInt& integer);
protected:
	virtual void CreateContent();	
private:				
	CFilterEdit m_edit;
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

