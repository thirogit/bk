#ifndef __STRINGOPTIONCTRL_H__
#define __STRINGOPTIONCTRL_H__

#include "DocumentOptionCtrl.h"
#include <vector>
#include <types\Decimal.h>
#include "../../../ctrls/stdedit/stdedit.h"

class StringOptionCtrl : public DocumentOptionCtrl
{	
public:
	StringOptionCtrl();
	~StringOptionCtrl();

	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	std::wstring GetString() const;
	void SetString(const std::wstring& str);
protected:
	virtual void CreateContent();	
private:	
			
	CStdEdit m_edit;		
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

