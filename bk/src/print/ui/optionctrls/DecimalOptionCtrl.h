#ifndef __DECIMALOPTIONCTRL_H__
#define __DECIMALOPTIONCTRL_H__

#include "DocumentOptionCtrl.h"
#include <types\NullDecimal.h>
#include "../../../ctrls/decimaledit/DecimalEdit.h"

class DecimalOptionCtrl : public DocumentOptionCtrl
{	
public:
	DecimalOptionCtrl();
	~DecimalOptionCtrl();

	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	NullDecimal GetDecimal() const;
	void SetDecimal(const NullDecimal& decimal);
protected:
	virtual void CreateContent();	
private:				
	CDecimalEdit m_edit;
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

